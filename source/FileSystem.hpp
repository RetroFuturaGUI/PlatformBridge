#pragma once
#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace PlatformBridge
{
    class FileSystem
    {
    public:
        enum class EntryKind : uint32_t
        {
            File,
            Directory,
            Other //devices, sockets, broken links
        };

        enum class RootKind : uint32_t
        {
            Fixed,
            Removable,
            Optical,
            Network,
            Other
        };

        enum class KnownFolder : uint32_t
        {
            Home,
            Desktop,
            Documents,
            Downloads,
            Music,
            Pictures,
            Videos
        };

        /// @brief What MoveToTrash did with an item.
        enum class TrashResult : uint32_t
        {
            Moved,
            NotTrashable, //the bin can't take it: a drive without one, or an item too big for it. It is left where it is.
            Failed
        };

        /// @brief One file or folder found in a listed folder.
        struct Entry
        {
            std::filesystem::path _Path {};
            EntryKind _Kind { EntryKind::Other };
            uint64_t _Size { 0 }; //in bytes, 0 for folders
            std::filesystem::file_time_type _LastWriteTime {};
            bool
                _IsHidden { false },
                _IsOnlineOnly { false }; //a cloud file whose content isn't on this disk: reading it downloads all of it (Windows)
        };

        /// @brief Watches one folder for changes to what it holds: files and folders added, removed or renamed, their sizes, write
        ///        times and attributes, not what happens inside its subfolders. Poll it once per frame; it never waits.
        class DirectoryWatcher
        {
        public:
            DirectoryWatcher() = default;
            DirectoryWatcher(const DirectoryWatcher&) = delete;
            DirectoryWatcher(DirectoryWatcher&&) = delete;
            DirectoryWatcher& operator=(const DirectoryWatcher&) = delete;
            DirectoryWatcher& operator=(DirectoryWatcher&&) = delete;
            ~DirectoryWatcher();

            /// @brief Watches a folder instead of the one watched before.
            /// @param errorOut Set when it can't be watched: the folder is gone, access is denied, or the system has no watch left.
            /// @return false when it can't be watched; nothing is watched then.
            bool Watch(const std::filesystem::path& directory, std::error_code& errorOut);

            /// @brief Stops watching.
            void Stop();

            /// @brief Returns whether the folder changed since the last call. Any number of changes in between count as one.
            ///        When the folder itself goes away the watch ends, after this has returned true once more.
            bool HasChanged();

            bool IsWatching() const;

        private:
#ifdef _WIN32
            void* _handle { nullptr }; //the change notification's HANDLE, nullptr while nothing is watched
#elif __linux__
            int
                _inotify { -1 }, //the inotify instance, -1 while nothing is watched
                _watch { -1 };
#endif
        };

        /// @brief A place browsing starts from: a drive on Windows, the file system root or a mounted volume on Linux.
        struct Root
        {
            std::filesystem::path _Path {}; //"C:\", "/", "/media/user/stick"
            std::string _Label {};          //UTF-8 volume label, may be empty
            RootKind _Kind { RootKind::Other };
        };

        /// @brief Re-reads the roots and known folders.
        static void Refresh();

        /// @brief Returns the roots found by the last Refresh.
        static const std::vector<Root>& GetRoots();

        /// @brief Returns the given known folder, read by the last Refresh.
        /// @return An empty path when the platform has no such folder.
        static const std::filesystem::path& GetKnownFolder(const KnownFolder folder);

        /// @brief Lists the files and folders in a folder, without "." and "..". Never throws.
        /// @param entriesOut Cleared, then filled in the order the platform returns them.
        /// @param errorOut Set when the folder can't be listed: access denied, drive not ready, network path gone.
        /// @return false when the folder can't be listed.
        static bool ListDirectory(const std::filesystem::path& directory, std::vector<Entry>& entriesOut, std::error_code& errorOut);

        /// @brief Moves a file or folder to the recycle bin on Windows, to the trash in the home folder on Linux. Never shows a
        ///        dialog and never deletes for good: an item the bin can't take is left alone, so the caller can ask first. On
        ///        Linux only items on the home folder's file system can go. Blocks until done; a big folder on a slow drive takes
        ///        a while, so call it off the main thread. Windows initializes COM on the calling thread for the call.
        /// @param errorOut Set when it fails: the item is gone, in use, or access is denied.
        static TrashResult MoveToTrash(const std::filesystem::path& path, std::error_code& errorOut);

        /// @brief Puts back the item most recently moved to the recycle bin (Windows) or the trash (Linux) from this path. A name
        ///        taken there meanwhile gets a free one, "Name (2)"; a folder it was in that went too is made again. Blocks
        ///        while it searches the bin, so call it off the main thread.
        /// @param restoredPathOut Where it is now: the original path, or the free name it got.
        /// @param errorOut no_such_file_or_directory when the bin holds nothing from this path, or why it can't go back
        static bool RestoreFromTrash(const std::filesystem::path& originalPath, std::filesystem::path& restoredPathOut, std::error_code& errorOut);

        /// @brief The path with a name nothing in its folder has yet: as it is, or "Name (2).ext", "Name (3).ext" and so on. A
        ///        folder's number goes at the end of its name, since a dot in it starts no extension. A broken link takes its
        ///        name too.
        static std::filesystem::path FreeName(const std::filesystem::path& wanted, const bool isFolder)
        {
            std::error_code error {};

            if(!std::filesystem::exists(std::filesystem::symlink_status(wanted, error)))
                return wanted;

            const std::filesystem::path
                folder { wanted.parent_path() },
                base { isFolder ? wanted.filename() : wanted.stem() },
                extension { isFolder ? std::filesystem::path {} : wanted.extension() };

            for(uint32_t number = 2; _freeNameLimit > number; ++number)
            {
                std::filesystem::path candidate { folder / base };
                candidate += " (" + std::to_string(number) + ")";
                candidate += extension;

                if(!std::filesystem::exists(std::filesystem::symlink_status(candidate, error)))
                    return candidate;
            }

            return wanted; //every number taken: the caller finds the name taken and says so
        }

        /// @brief Returns whether two existing files or folders lie on the same volume, so one can be renamed into the other's
        ///        place instead of copied. Mount points and subst drives count as the volume they show. True when it can't tell.
        static bool IsSameVolume(const std::filesystem::path& first, const std::filesystem::path& second);

    private:
        FileSystem() = delete;

        static void refreshRoots();
        static void refreshKnownFolders();
#ifdef _WIN32
        class TrashSink; //hears from the Shell, before an item is deleted, whether it goes to the recycle bin

        /// @brief An HRESULT as an error code with a message: a Win32 error as itself, the Shell's copy engine errors, which the
        ///        system has no text for, as the Win32 error they mean.
        static std::error_code shellError(const long result);

        /// @brief Returns whether the item fits its volume's recycle bin, which the Shell checks only by deleting a bigger one for
        ///        good. False too where the bin is switched off. True when it can't tell, so the Shell's own checks decide.
        static bool fitsRecycleBin(const std::filesystem::path& path);
#elif __linux__
        /// @brief $XDG_DATA_HOME/Trash, or ~/.local/share/Trash; empty without a home.
        static std::filesystem::path trashDirectory();

        /// @brief A path as a trash info file writes it: percent-encoded, every byte but letters, digits, "-._~" and "/".
        static std::string encodeTrashPath(const std::string& path);

        /// @brief A trash info file's path as it is: each "%XX" back to its byte.
        static std::string decodeTrashPath(std::string_view text);
#endif

        static constexpr size_t _knownFolderCount { static_cast<size_t>(KnownFolder::Videos) + 1 };
        static constexpr uint32_t _freeNameLimit { 10000 }; //the numbers FreeName tries before giving up

        static inline std::vector<Root> _roots {};
        static inline std::array<std::filesystem::path, _knownFolderCount> _knownFolders {};
    };
}