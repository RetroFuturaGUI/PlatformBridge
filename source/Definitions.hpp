/* This header forwards platform-specific definitions to one unified format to ensure integrity to all target platforms.
    lots of thanks to the x11 community

*/
//#ifdef __linux__  //
#if defined(TARGET_PLATFORM_LINUX)
    #include <X11/keysym.h>
    #define PB_KEY_VoidSymbol XK_VoidSymbol
    #ifdef XK_MISCELLANY

    #define PB_KEY_BACKSPACE XK_BackSpace
    #define PB_KEY_TAB XK_Tab
    #define PB_KEY_LINEFEED XK_Linefeed
    #define PB_KEY_CLEAR XK_Clear
    #define PB_KEY_RETURN XK_Return
    #define PB_KEY_ENTER XK_Return
    #define PB_KEY_PAUSE XK_Pause
    #define PB_KEY_SCROLL_LOCK XK_Scroll_Lock
    #define PB_KEY_SYS_REQ XK_Sys_Req
    #define PB_KEY_ESCAPE XK_Escape
    #define PB_KEY_DELETE XK_Delete

    /* International & multi-key character composition */

    #define PB_KEY_MULTI XK_Multi_key
    #define PB_KEY_CODEINPUT XK_Codeinput
    #define PB_KEY_SINGLE_CANDITATE XK_SingleCandidate
    #define PB_KEY_MULTIPLE_CANDIDATE XK_MultipleCandidate
    #define PB_KEY_PREVIOUS_CANDIDATE XK_PreviousCandidate

    /* Japanese keyboard support */

    #define PB_KEY_KANJI XK_Kanji
    #define PB_KEY_MUHENKAN XK_Muhenkan
    #define PB_KEY_HENKAN_MODE XK_Henkan_Mode
    #define PB_KEY_HENKAN XK_Henkan
    #define PB_KEY_ROMAJI XK_Romaji
    #define PB_KEY_HIRAGANA XK_Hiragana
    #define PB_KEY_KATAKANA XK_Katakana
    #define PB_KEY_HIRAGANA_KATAKANA XK_Hiragana_Katakana
    #define PB_KEY_ZENKAKU XK_Zenkaku
    #define PB_KEY_HANKAKU XK_Hankaku
    #define PB_KEY_ZENKAKU_HANKAKU XK_Zenkaku_Hankaku
    #define PB_KEY_TOUROKU XK_Touroku
    #define PB_KEY_MASSYO XK_Massyo
    #define PB_KEY_KANA_LOCK XK_Kana_Lock
    #define PB_KEY_KANA_SHIFT XK_Kana_Shift
    #define PB_KEY_EISU_SHIFT XK_Eisu_Shift
    #define PB_KEY_EISU_TOGGLE XK_Eisu_toggle
    #define PB_KEY_KANJI_BANGOU XK_Kanji_Bangou
    #define PB_KEY_ZEN_KOHO XK_Zen_Koho
    #define PB_KEY_MAE_KOHO XK_Mae_Koho

    /*0xff31 thru 0xff3f are under PB_KEY_ KOREAN */

    /* Cursor control & motion */ 

    #define PB_KEY_HOME XK_Home
    #define PB_KEY_LEFT XK_Left
    #define PB_KEY_UP XK_Up
    #define PB_KEY_RIGHT XK_Right
    #define PB_KEY_DOWN XK_Down
    #define PB_KEY_PRIOR XK_Prior
    #define PB_KEY_PAGE_UP XK_Page_Up
    #define PB_KEY_NEXT XK_Next
    #define PB_KEY_PAGE_DOWN XK_Page_Down
    #define PB_KEY_END XK_End
    #define PB_KEY_BEGIN XK_Begin


    //MiscFUNCTIONSfunctions

    #define PB_KEY_SELECT XK_Select
    #define PB_KEY_PRINT XK_Print
    #define PB_KEY_EXECUTE XK_Execute
    #define PB_KEY_INSERT XK_Insert
    #define PB_KEY_UNDO XK_Undo
    #define PB_KEY_REDO XK_Redo
    #define PB_KEY_MENU XK_Menu
    #define PB_KEY_FIND XK_Find
    #define PB_KEY_CANCEL XK_Cancel
    #define PB_KEY_HELP XK_Help
    #define PB_KEY_BREAK XK_Break
    #define PB_KEY_MODE_SWITCH XK_Mode_switch
    #define PB_KEY_SCRIPT_SWITCH XK_script_switch
    #define PB_KEY_NUMLOCK XK_Num_Lock

    /* Keypad functions, keypad numbers cleverly chosen to map to ASCII */

    #define PB_KEY_KP_SPACE XK_KP_Space
    #define PB_KEY_KP_TAB XK_KP_Tab
    #define PB_KEY_KP_ENTER XK_KP_Enter
    #define PB_KEY_KP_F1 XK_KP_F1
    #define PB_KEY_KP_F2 XK_KP_F2
    #define PB_KEY_KP_F3 XK_KP_F3
    #define PB_KEY_KP_F4 XK_KP_F4
    #define PB_KEY_KP_HOME XK_KP_Home
    #define PB_KEY_KP_LEFT XK_KP_Left
    #define PB_KEY_KP_UP XK_KP_Up
    #define PB_KEY_KP_RIGHT XK_KP_Right
    #define PB_KEY_KP_DOWN XK_KP_Down
    #define PB_KEY_KP_PRIOR XK_KP_Prior
    #define PB_KEY_KP_PAGE_UP XK_KP_Page_Up
    #define PB_KEY_KP_NEXT XK_KP_Next
    #define PB_KEY_KP_PAGE_DOWN XK_KP_Page_Down
    #define PB_KEY_KP_END XK_KP_End
    #define PB_KEY_KP_BEGIN XK_KP_Begin
    #define PB_KEY_KP_INSERT XK_KP_Insert
    #define PB_KEY_KP_DELETE XK_KP_Delete
    #define PB_KEY_KP_EQUAL XK_KP_Equal
    #define PB_KEY_KP_MULTIPLY XK_KP_Multiply
    #define PB_KEY_KP_ADD XK_KP_Add
    #define PB_KEY_KP_SEPARATOR XK_KP_Separator
    #define PB_KEY_KP_SUBTRACT XK_KP_Subtract
    #define PB_KEY_KP_DECIMAL XK_KP_Decimal
    #define PB_KEY_KP_DIVIDE XK_KP_Divide

    #define PB_KEY_KP_0 XK_KP_0
    #define PB_KEY_KP_1 XK_KP_1
    #define PB_KEY_KP_2 XK_KP_2
    #define PB_KEY_KP_3 XK_KP_3
    #define PB_KEY_KP_4 XK_KP_4
    #define PB_KEY_KP_5 XK_KP_5
    #define PB_KEY_KP_6 XK_KP_6
    #define PB_KEY_KP_7 XK_KP_7
    #define PB_KEY_KP_8 XK_KP_8
    #define PB_KEY_KP_9 XK_KP_9

    /** Auxiliary functions; note the duplicate definitions for left and right   * function keys;  Sun keyboards and a few other manufacturers have such * function key groups on the left and/or right sides of the keyboard. * We've not found a keyboard with more than 35 function keys total. */             


    #define PB_KEY_F1 XK_F1
    #define PB_KEY_F2 XK_F2
    #define PB_KEY_F3 XK_F3
    #define PB_KEY_F4 XK_F4
    #define PB_KEY_F5 XK_F5
    #define PB_KEY_F6 XK_F6
    #define PB_KEY_F7 XK_F7
    #define PB_KEY_F8 XK_F8
    #define PB_KEY_F9 XK_F9
    #define PB_KEY_F10 XK_F10
    #define PB_KEY_F11 XK_F11
    #define PB_KEY_L1 XK_L1
    #define PB_KEY_F12 XK_F12
    #define PB_KEY_L2 XK_L2
    #define PB_KEY_F13 XK_F13
    #define PB_KEY_L3 XK_L3
    #define PB_KEY_F14 XK_F14
    #define PB_KEY_L4 XK_L4
    #define PB_KEY_F15 XK_F15
    #define PB_KEY_L5 XK_L5
    #define PB_KEY_F16 XK_F16
    #define PB_KEY_L6 XK_L6
    #define PB_KEY_F17 XK_F17
    #define PB_KEY_L7 XK_L7
    #define PB_KEY_F18 XK_F18
    #define PB_KEY_L8 XK_L8
    #define PB_KEY_F19 XK_F19
    #define PB_KEY_L9 XK_L9
    #define PB_KEY_F20 XK_F20
    #define PB_KEY_L10 XK_L10
    #define PB_KEY_F21 XK_F21
    #define PB_KEY_R1 XK_R1
    #define PB_KEY_F22 XK_F22
    #define PB_KEY_R2 XK_R2
    #define PB_KEY_F23 XK_F23
    #define PB_KEY_R3 XK_R3
    #define PB_KEY_F24 XK_F24
    #define PB_KEY_R4 XK_R4
    #define PB_KEY_F25 XK_F25
    #define PB_KEY_R5 XK_R5
    #define PB_KEY_F26 XK_F26
    #define PB_KEY_R6 XK_R6
    #define PB_KEY_F27 XK_F27
    #define PB_KEY_R7 XK_R7
    #define PB_KEY_F28 XK_F28
    #define PB_KEY_R8 XK_R8
    #define PB_KEY_F29 XK_F29
    #define PB_KEY_R9 XK_R9
    #define PB_KEY_F30 XK_F30
    #define PB_KEY_R10 XK_R10
    #define PB_KEY_F31 XK_F31
    #define PB_KEY_R11 XK_R11
    #define PB_KEY_F32 XK_F32
    #define PB_KEY_R12 XK_R12
    #define PB_KEY_F33 XK_F33
    #define PB_KEY_R13 XK_R13
    #define PB_KEY_F34 XK_F34
    #define PB_KEY_R14 XK_R14
    #define PB_KEY_F35 XK_F35
    #define PB_KEY_R15 XK_R15

    /*Modifiers*/

    #define PB_KEY_SHIFT_L XK_Shift_L
    #define PB_KEY_SHIFT_R XK_Shift_R
    #define PB_KEY_CONTROL_L XK_Control_L
    #define PB_KEY_CONTROL_R XK_Control_R
    #define PB_KEY_CAPS_LOCK XK_Caps_Lock
    #define PB_KEY_SHIFT_LOCK XK_Shift_Lock

    #define PB_KEY_META_L XK_Meta_L
    #define PB_KEY_META_R XK_Meta_R
    #define PB_KEY_ALT_L XK_Alt_L
    #define PB_KEY_ALT_R XK_Alt_R
    #define PB_KEY_SUPER_L XK_Super_L
    #define PB_KEY_SUPER_R XK_Super_R
    #define PB_KEY_HYPER_L XK_Hyper_L
    #define PB_KEY_HYPER_R XK_Hyper_R
    #endif

    //            * Keyboard (XKB) Extension function and modifier keys     * (from Appendix C of "The X Keyboard Extension: Protocol Specification")  * Byte 3 = 0xfe        */           


    #ifdef XK_XKB_KEYS
    #define PB_KEY_ISO_LOCK XK_ISO_Lock
    #define PB_KEY_ISO_LEVEL2_LATCH XK_ISO_Level2_Latch
    #define PB_KEY_ISO_LEVEL3_SHIFT XK_ISO_Level3_Shift
    #define PB_KEY_ISO_LEVEL3_LATCH XK_ISO_Level3_Latch
    #define PB_KEY_ISO_LEVEL3_LOCK XK_ISO_Level3_Lock
    #define PB_KEY_ISO_LEVEL5_SHIFT XK_ISO_Level5_Shift
    #define PB_KEY_ISO_LEVEL5_LATCH XK_ISO_Level5_Latch
    #define PB_KEY_ISO_LEVEL5_LOCK XK_ISO_Level5_Lock
    #define PB_KEY_ISO_GROUP_SHIFT XK_ISO_Group_Shift
    #define PB_KEY_ISO_GROUP_LATCH XK_ISO_Group_Latch
    #define PB_KEY_ISO_GROUP_LOCK XK_ISO_Group_Lock
    #define PB_KEY_ISO_NEXT_GROUP XK_ISO_Next_Group
    #define PB_KEY_ISO_NEXT_GROUP_LOCK XK_ISO_Next_Group_Lock
    #define PB_KEY_ISO_PREV_GROUP XK_ISO_Prev_Group
    #define PB_KEY_ISO_PREV_GROUP_LOCK XK_ISO_Prev_Group_Lock
    #define PB_KEY_ISO_FIRST_GROUP XK_ISO_First_Group
    #define PB_KEY_ISO_FIRST_GROUP_LOCK XK_ISO_First_Group_Lock
    #define PB_KEY_ISO_LAST_GROUP XK_ISO_Last_Group
    #define PB_KEY_ISO_LAST_GROUP_LOCK XK_ISO_Last_Group_Lock

    #define PB_KEY_ISO_LEFT_TAB XK_ISO_Left_Tab
    #define PB_KEY_ISO_MOVE_LINE_UP XK_ISO_Move_Line_Up
    #define PB_KEY_ISO_MOVE_LINE_DOWN XK_ISO_Move_Line_Down
    #define PB_KEY_ISO_PARTIAL_LINE_UP XK_ISO_Partial_Line_Up
    #define PB_KEY_ISO_PARTIAL_LINE_DOWN XK_ISO_Partial_Line_Down
    #define PB_KEY_ISO_PARTIAL_SPACE_LEFT XK_ISO_Partial_Space_Left
    #define PB_KEY_ISO_PARTIAL_SPACE_RIGHT XK_ISO_Partial_Space_Right
    #define PB_KEY_ISO_SET_MARGIN_LEFT XK_ISO_Set_Margin_Left
    #define PB_KEY_ISO_SET_MARGIN_RIGHT XK_ISO_Set_Margin_Right
    #define PB_KEY_ISO_RELEASE_MARGIN_LEFT XK_ISO_Release_Margin_Left
    #define PB_KEY_ISO_RELEASE_MARGIN_RIGHT XK_ISO_Release_Margin_Right
    #define PB_KEY_ISO_RELEASE_BOTH_MARGINS XK_ISO_Release_Both_Margins
    #define PB_KEY_ISO_FAST_CURSOR_LEFT XK_ISO_Fast_Cursor_Left
    #define PB_KEY_ISO_FAST_CURSOR_RIGHT XK_ISO_Fast_Cursor_Right
    #define PB_KEY_ISO_FAST_CURSOR_UP XK_ISO_Fast_Cursor_Up
    #define PB_KEY_ISO_FAST_CURSOR_DOWN XK_ISO_Fast_Cursor_Down
    #define PB_KEY_ISO_CONTINUOUS_UNDERLINE XK_ISO_Continuous_Underline
    #define PB_KEY_ISO_DISCONTINUOUS_UNDERLINE XK_ISO_Discontinuous_Underline
    #define PB_KEY_ISO_EMPHASIZE XK_ISO_Emphasize
    #define PB_KEY_ISO_CENTER_OBJECT XK_ISO_Center_Object
    #define PB_KEY_ISO_ENTER XK_ISO_Enter

    #define PB_KEY_DEAD_GRAVE XK_dead_grave
    #define PB_KEY_DEAD_ACUTE XK_dead_acute
    #define PB_KEY_DEAD_CIRCUMFLEX XK_dead_circumflex
    #define PB_KEY_DEAD_TILDE XK_dead_tilde
    #define PB_KEY_DEAD_PERISPOMENI XK_dead_perispomeni
    #define PB_KEY_DEAD_MACRON XK_dead_macron
    #define PB_KEY_DEAD_BREVE XK_dead_breve
    #define PB_KEY_DEAD_ABOVEDOT XK_dead_abovedot
    #define PB_KEY_DEAD_DIAERESIS XK_dead_diaeresis
    #define PB_KEY_DEAD_ABOVERING XK_dead_abovering
    #define PB_KEY_DEAD_DOUBLEACUTE XK_dead_doubleacute
    #define PB_KEY_DEAD_CARON XK_dead_caron
    #define PB_KEY_DEAD_CEDILLA XK_dead_cedilla
    #define PB_KEY_DEAD_OGONEK XK_dead_ogonek
    #define PB_KEY_DEAD_IOTA XK_dead_iota
    #define PB_KEY_DEAD_VOICED_SOUND XK_dead_voiced_sound
    #define PB_KEY_DEAD_SEMIVOICED_SOUND XK_dead_semivoiced_sound
    #define PB_KEY_DEAD_BELOWDOT XK_dead_belowdot
    #define PB_KEY_DEAD_HOOK XK_dead_hook
    #define PB_KEY_DEAD_HORN XK_dead_horn
    #define PB_KEY_DEAD_STROKE XK_dead_stroke
    #define PB_KEY_DEAD_ABOVECOMMA XK_dead_abovecomma
    #define PB_KEY_DEAD_PSILI XK_dead_psili
    #define PB_KEY_DEAD_ABOVEREVERSEDCOMMA XK_dead_abovereversedcomma
    #define PB_KEY_DEAD_DASIA XK_dead_dasia
    #define PB_KEY_DEAD_DOUBLEGRAVE XK_dead_doublegrave
    #define PB_KEY_DEAD_BELOWRING XK_dead_belowring
    #define PB_KEY_DEAD_BELOWMACRON XK_dead_belowmacron
    #define PB_KEY_DEAD_BELOWCIRCUMFLEX XK_dead_belowcircumflex
    #define PB_KEY_DEAD_BELOWTILDE XK_dead_belowtilde
    #define PB_KEY_DEAD_BELOWBREVE XK_dead_belowbreve
    #define PB_KEY_DEAD_BELOWDIAERESIS XK_dead_belowdiaeresis
    #define PB_KEY_DEAD_INVERTEDBREVE XK_dead_invertedbreve
    #define PB_KEY_DEAD_BELOWCOMMA XK_dead_belowcomma
    #define PB_KEY_DEAD_CURRENCY XK_dead_currency

    /*extra dead elements for German T3 layout */ 
    #define PB_KEY_DEAD_LOWLINE XK_dead_lowline
    #define PB_KEY_DEAD_ABOVEVERTICALLINE XK_dead_aboveverticalline
    #define PB_KEY_DEAD_BELOWVERTICALLINE XK_dead_belowverticalline
    #define PB_KEY_DEAD_LONGSOLIDUSOVERLAY XK_dead_longsolidusoverlay

    /*dead vowels for universal syllable entry */ 
    #define PB_KEY_DEAD_A XK_dead_a
    #define PB_KEY_DEAD_CAPITAL_A XK_dead_A
    #define PB_KEY_DEAD_E XK_dead_e
    #define PB_KEY_DEAD_CAPITAL_E XK_dead_E
    #define PB_KEY_DEAD_I XK_dead_i
    #define PB_KEY_DEAD_CAPITAL_I XK_dead_I
    #define PB_KEY_DEAD_O XK_dead_o
    #define PB_KEY_DEAD_CAPITAL_O XK_dead_O
    #define PB_KEY_DEAD_U XK_dead_u
    #define PB_KEY_DEAD_CAPITAL_U XK_dead_U
    #define PB_KEY_DEAD_SMALL_SCHWA XK_dead_small_schwa
    #define PB_KEY_DEAD_SCHWA XK_dead_schwa
    #define PB_KEY_DEAD_CAPITAL_SCHWA XK_dead_capital_schwa
    //#define PB_KEY_DEAD_SCHWA XK_dead_SCHWA

    #define PB_KEY_DEAD_GREEK XK_dead_greek
    #define PB_KEY_DEAD_HAMZA XK_dead_hamza

    #define PB_KEY_FIRST_VIRTUAL_SCREEN XK_First_Virtual_Screen
    #define PB_KEY_PREV_VIRTUAL_SCREEN XK_Prev_Virtual_Screen
    #define PB_KEY_NEXT_VIRTUAL_SCREEN XK_Next_Virtual_Screen
    #define PB_KEY_LAST_VIRTUAL_SCREEN XK_Last_Virtual_Screen
    #define PB_KEY_TERMINATE_SERVER XK_Terminate_Server

    #define PB_KEY_ACCESSX_ENABLE XK_AccessX_Enable
    #define PB_KEY_ACCESSX_FEEDBACK_ENABLE XK_AccessX_Feedback_Enable
    #define PB_KEY_REPEATKEYS_ENABLE XK_RepeatKeys_Enable
    #define PB_KEY_SLOWKEYS_ENABLE XK_SlowKeys_Enable
    #define PB_KEY_BOUNCEKEYS_ENABLE XK_BounceKeys_Enable
    #define PB_KEY_STICKYKEYS_ENABLE XK_StickyKeys_Enable
    #define PB_KEY_MOUSEKEYS_ENABLE XK_MouseKeys_Enable
    #define PB_KEY_MOUSEKEYS_ACCEL_ENABLE XK_MouseKeys_Accel_Enable
    #define PB_KEY_OVERLAY1_ENABLE XK_Overlay1_Enable
    #define PB_KEY_OVERLAY2_ENABLE XK_Overlay2_Enable
    #define PB_KEY_AUDIBLEBELL_ENABLE XK_AudibleBell_Enable

    #define PB_KEY_POINTER_LEFT XK_Pointer_Left
    #define PB_KEY_POINTER_RIGHT XK_Pointer_Right
    #define PB_KEY_POINTER_UP XK_Pointer_Up
    #define PB_KEY_POINTER_DOWN XK_Pointer_Down
    #define PB_KEY_POINTER_UPLEFT XK_Pointer_UpLeft
    #define PB_KEY_POINTER_UPRIGHT XK_Pointer_UpRight
    #define PB_KEY_POINTER_DOWNLEFT XK_Pointer_DownLeft
    #define PB_KEY_POINTER_DOWNRIGHT XK_Pointer_DownRight
    #define PB_KEY_POINTER_BUTTON_DFLT XK_Pointer_Button_Dflt
    #define PB_KEY_POINTER_BUTTON1 XK_Pointer_Button1
    #define PB_KEY_POINTER_BUTTON2 XK_Pointer_Button2
    #define PB_KEY_POINTER_BUTTON3 XK_Pointer_Button3
    #define PB_KEY_POINTER_BUTTON4 XK_Pointer_Button4
    #define PB_KEY_POINTER_BUTTON5 XK_Pointer_Button5
    #define PB_KEY_POINTER_DBLCLICK_DFLT XK_Pointer_DblClick_Dflt
    #define PB_KEY_POINTER_DBLCLICK1 XK_Pointer_DblClick1
    #define PB_KEY_POINTER_DBLCLICK2 XK_Pointer_DblClick2
    #define PB_KEY_POINTER_DBLCLICK3 XK_Pointer_DblClick3
    #define PB_KEY_POINTER_DBLCLICK4 XK_Pointer_DblClick4
    #define PB_KEY_POINTER_DBLCLICK5 XK_Pointer_DblClick5
    #define PB_KEY_POINTER_DRAG_DFLT XK_Pointer_Drag_Dflt
    #define PB_KEY_POINTER_DRAG1 XK_Pointer_Drag1
    #define PB_KEY_POINTER_DRAG2 XK_Pointer_Drag2
    #define PB_KEY_POINTER_DRAG3 XK_Pointer_Drag3
    #define PB_KEY_POINTER_DRAG4 XK_Pointer_Drag4
    #define PB_KEY_POINTER_DRAG5 XK_Pointer_Drag5

    #define PB_KEY_POINTER_ENABLEKEYS XK_Pointer_EnableKeys
    #define PB_KEY_POINTER_ACCELERATE XK_Pointer_Accelerate
    #define PB_KEY_POINTER_DFLTBTNNEXT XK_Pointer_DfltBtnNext
    #define PB_KEY_POINTER_DFLTBTNPREV XK_Pointer_DfltBtnPrev

    /*Single-Stroke Multiple-Character N-Graph Keysyms For The X Input Method */ 

    #define PB_KEY_CH XK_ch
    #define PB_KEY_SMALL_CH XK_Ch
    #define PB_KEY_CAPITAL_CH XK_CH
    #define PB_KEY_C_H XK_c_h
    #define PB_KEY_SMALL_C_H XK_C_h
    #define PB_KEY_CAPITAL_C_H XK_C_H
    #endif

    /* XKB_KEYS */        /*       * 3270 Terminal Keys   * Byte 3 = 0xfd  */     





    #ifdef XK_3270
    #define PB_KEY_3270_DUPLICATE XK_3270_Duplicate
    #define PB_KEY_3270_FIELDMARK XK_3270_FieldMark
    #define PB_KEY_3270_RIGHT2 XK_3270_Right2
    #define PB_KEY_3270_LEFT2 XK_3270_Left2
    #define PB_KEY_3270_BACKTAB XK_3270_BackTab
    #define PB_KEY_3270_ERASEEOF XK_3270_EraseEOF
    #define PB_KEY_3270_ERASEINPUT XK_3270_EraseInput
    #define PB_KEY_3270_RESET XK_3270_Reset
    #define PB_KEY_3270_QUIT XK_3270_Quit
    #define PB_KEY_3270_PA1 XK_3270_PA1
    #define PB_KEY_3270_PA2 XK_3270_PA2
    #define PB_KEY_3270_PA3 XK_3270_PA3
    #define PB_KEY_3270_TEST XK_3270_Test
    #define PB_KEY_3270_ATTN XK_3270_Attn
    #define PB_KEY_3270_CURSORBLINK XK_3270_CursorBlink
    #define PB_KEY_3270_ALTCURSOR XK_3270_AltCursor
    #define PB_KEY_3270_KEYCLICK XK_3270_KeyClick
    #define PB_KEY_3270_JUMP XK_3270_Jump
    #define PB_KEY_3270_IDENT XK_3270_Ident
    #define PB_KEY_3270_RULE XK_3270_Rule
    #define PB_KEY_3270_COPY XK_3270_Copy
    #define PB_KEY_3270_PLAY XK_3270_Play
    #define PB_KEY_3270_SETUP XK_3270_Setup
    #define PB_KEY_3270_RECORD XK_3270_Record
    #define PB_KEY_3270_CHANGESCREEN XK_3270_ChangeScreen
    #define PB_KEY_3270_DELETEWORD XK_3270_DeleteWord
    #define PB_KEY_3270_EXSELECT XK_3270_ExSelect
    #define PB_KEY_3270_CURSORSELECT XK_3270_CursorSelect
    #define PB_KEY_3270_PRINTSCREEN XK_3270_PrintScreen
    #define PB_KEY_3270_ENTER XK_3270_Enter
    #endif

    /*             * Latin 1    * (ISO/IEC 8859-1 = Unicode U+0020..U+00FF) * Byte 3 = 0  */      




    #ifdef XK_LATIN1
    #define PB_KEY_SPACE XK_space
    #define PB_KEY_EXCLAM XK_exclam
    #define PB_KEY_QUOTEDBL XK_quotedbl
    #define PB_KEY_NUMBERSIGN XK_numbersign
    #define PB_KEY_DOLLAR XK_dollar
    #define PB_KEY_PERCENT XK_percent
    #define PB_KEY_AMPERSAND XK_ampersand
    #define PB_KEY_APOSTROPHE XK_apostrophe
    #define PB_KEY_QUOTERIGHT XK_quoteright
    #define PB_KEY_PARENLEFT XK_parenleft
    #define PB_KEY_PARENRIGHT XK_parenright
    #define PB_KEY_ASTERISK XK_asterisk
    #define PB_KEY_PLUS XK_plus
    #define PB_KEY_COMMA XK_comma
    #define PB_KEY_MINUS XK_minus
    #define PB_KEY_PERIOD XK_period
    #define PB_KEY_SLASH XK_slash
    #define PB_KEY_0 XK_0
    #define PB_KEY_1 XK_1
    #define PB_KEY_2 XK_2
    #define PB_KEY_3 XK_3
    #define PB_KEY_4 XK_4
    #define PB_KEY_5 XK_5
    #define PB_KEY_6 XK_6
    #define PB_KEY_7 XK_7
    #define PB_KEY_8 XK_8
    #define PB_KEY_9 XK_9
    #define PB_KEY_COLON XK_colon
    #define PB_KEY_SEMICOLON XK_semicolon
    #define PB_KEY_LESS XK_less
    #define PB_KEY_EQUAL XK_equal
    #define PB_KEY_GREATER XK_greater
    #define PB_KEY_QUESTION XK_question
    #define PB_KEY_AT XK_at
    #define PB_KEY_CAPITAL_A XK_A
    #define PB_KEY_CAPITAL_B XK_B
    #define PB_KEY_CAPITAL_C XK_C
    #define PB_KEY_CAPITAL_D XK_D
    #define PB_KEY_CAPITAL_E XK_E
    #define PB_KEY_CAPITAL_F XK_F
    #define PB_KEY_CAPITAL_G XK_G
    #define PB_KEY_CAPITAL_H XK_H
    #define PB_KEY_CAPITAL_I XK_I
    #define PB_KEY_CAPITAL_J XK_J
    #define PB_KEY_CAPITAL_K XK_K
    #define PB_KEY_CAPITAL_L XK_L
    #define PB_KEY_CAPITAL_M XK_M
    #define PB_KEY_CAPITAL_N XK_N
    #define PB_KEY_CAPITAL_O XK_O
    #define PB_KEY_CAPITAL_P XK_P
    #define PB_KEY_CAPITAL_Q XK_Q
    #define PB_KEY_CAPITAL_R XK_R
    #define PB_KEY_CAPITAL_S XK_S
    #define PB_KEY_CAPITAL_T XK_T
    #define PB_KEY_CAPITAL_U XK_U
    #define PB_KEY_CAPITAL_V XK_V
    #define PB_KEY_CAPITAL_W XK_W
    #define PB_KEY_CAPITAL_X XK_X
    #define PB_KEY_CAPITAL_Y XK_Y
    #define PB_KEY_CAPITAL_Z XK_Z
    #define PB_KEY_BRACKETLEFT XK_bracketleft
    #define PB_KEY_BACKSLASH XK_backslash
    #define PB_KEY_BRACKETRIGHT XK_bracketright
    #define PB_KEY_ASCIICIRCUM XK_asciicircum
    #define PB_KEY_UNDERSCORE XK_underscore
    #define PB_KEY_GRAVE XK_grave
    #define PB_KEY_QUOTELEFT XK_quoteleft
    #define PB_KEY_A XK_a
    #define PB_KEY_B XK_b
    #define PB_KEY_C XK_c
    #define PB_KEY_D XK_d
    #define PB_KEY_E XK_e
    #define PB_KEY_F XK_f
    #define PB_KEY_G XK_g
    #define PB_KEY_H XK_h
    #define PB_KEY_I XK_i
    #define PB_KEY_J XK_j
    #define PB_KEY_K XK_k
    #define PB_KEY_L XK_l
    #define PB_KEY_M XK_m
    #define PB_KEY_N XK_n
    #define PB_KEY_O XK_o
    #define PB_KEY_P XK_p
    #define PB_KEY_Q XK_q
    #define PB_KEY_R XK_r
    #define PB_KEY_S XK_s
    #define PB_KEY_T XK_t
    #define PB_KEY_U XK_u
    #define PB_KEY_V XK_v
    #define PB_KEY_W XK_w
    #define PB_KEY_X XK_x
    #define PB_KEY_Y XK_y
    #define PB_KEY_Z XK_z
    #define PB_KEY_BRACELEFT XK_braceleft
    #define PB_KEY_BAR XK_bar
    #define PB_KEY_BRACERIGHT XK_braceright
    #define PB_KEY_ASCIITILDE XK_asciitilde

    #define PB_KEY_NOBREAKSPACE XK_nobreakspace
    #define PB_KEY_EXCLAMDOWN XK_exclamdown
    #define PB_KEY_CENT XK_cent
    #define PB_KEY_STERLING XK_sterling
    #define PB_KEY_CURRENCY XK_currency
    #define PB_KEY_YEN XK_yen
    #define PB_KEY_BROKENBAR XK_brokenbar
    #define PB_KEY_SECTION XK_section
    #define PB_KEY_DIAERESIS XK_diaeresis
    #define PB_KEY_COPYRIGHT XK_copyright
    #define PB_KEY_ORDFEMININE XK_ordfeminine
    #define PB_KEY_GUILLEMOTLEFT XK_guillemotleft
    #define PB_KEY_GUILLEMETLEFT XK_guillemetleft
    #define PB_KEY_NOTSIGN XK_notsign
    #define PB_KEY_HYPHEN XK_hyphen
    #define PB_KEY_REGISTERED XK_registered
    #define PB_KEY_MACRON XK_macron
    #define PB_KEY_DEGREE XK_degree
    #define PB_KEY_PLUSMINUS XK_plusminus
    #define PB_KEY_TWOSUPERIOR XK_twosuperior
    #define PB_KEY_THREESUPERIOR XK_threesuperior
    #define PB_KEY_ACUTE XK_acute
    #define PB_KEY_MU XK_mu
    #define PB_KEY_PARAGRAPH XK_paragraph
    #define PB_KEY_PERIODCENTERED XK_periodcentered
    #define PB_KEY_CEDILLA XK_cedilla
    #define PB_KEY_ONESUPERIOR XK_onesuperior
    #define PB_KEY_MASCULINE XK_masculine
    #define PB_KEY_ORDMASCULINE XK_ordmasculine
    #define PB_KEY_GUILLEMOTRIGHT XK_guillemotright
    #define PB_KEY_GUILLEMETRIGHT XK_guillemetright
    #define PB_KEY_ONEQUARTER XK_onequarter
    #define PB_KEY_ONEHALF XK_onehalf
    #define PB_KEY_THREEQUARTERS XK_threequarters
    #define PB_KEY_QUESTIONDOWN XK_questiondown
    #define PB_KEY_CAPITAL_AGRAVE XK_Agrave
    #define PB_KEY_CAPITAL_AACUTE XK_Aacute
    #define PB_KEY_CAPITAL_ACIRCUMFLEX XK_Acircumflex
    #define PB_KEY_CAPITAL_ATILDE XK_Atilde
    #define PB_KEY_CAPITAL_ADIAERESIS XK_Adiaeresis
    #define PB_KEY_CAPITAL_ARING XK_Aring
    #define PB_KEY_CAPITAL_AE XK_AE
    #define PB_KEY_CAPITAL_CCEDILLA XK_Ccedilla
    #define PB_KEY_CAPITAL_EGRAVE XK_Egrave
    #define PB_KEY_CAPITAL_EACUTE XK_Eacute
    #define PB_KEY_CAPITAL_ECIRCUMFLEX XK_Ecircumflex
    #define PB_KEY_CAPITAL_EDIAERESIS XK_Ediaeresis
    #define PB_KEY_CAPITAL_IGRAVE XK_Igrave
    #define PB_KEY_CAPITAL_IACUTE XK_Iacute
    #define PB_KEY_CAPITAL_ICIRCUMFLEX XK_Icircumflex
    #define PB_KEY_CAPITAL_IDIAERESIS XK_Idiaeresis
    #define PB_KEY_CAPITAL_ETH XK_ETH
    //#define PB_KEY_ETH XK_Eth
    #define PB_KEY_CAPITAL_NTILDE XK_Ntilde
    #define PB_KEY_CAPITAL_OGRAVE XK_Ograve
    #define PB_KEY_CAPITAL_OACUTE XK_Oacute
    #define PB_KEY_CAPITAL_OCIRCUMFLEX XK_Ocircumflex
    #define PB_KEY_CAPITAL_OTILDE XK_Otilde
    #define PB_KEY_CAPITAL_ODIAERESIS XK_Odiaeresis
    #define PB_KEY_MULTIPLY XK_multiply
    #define PB_KEY_CAPITAL_OSLASH XK_Oslash
    #define PB_KEY_CAPITAL_OOBLIQUE XK_Ooblique
    #define PB_KEY_CAPITAL_UGRAVE XK_Ugrave
    #define PB_KEY_CAPITAL_UACUTE XK_Uacute
    #define PB_KEY_CAPITAL_UCIRCUMFLEX XK_Ucircumflex
    #define PB_KEY_CAPITAL_UDIAERESIS XK_Udiaeresis
    #define PB_KEY_CAPITAL_YACUTE XK_Yacute
    #define PB_KEY_CAPITAL_THORN XK_THORN
    //#define PB_KEY_THORN XK_Thorn
    #define PB_KEY_SSHARP XK_ssharp
    #define PB_KEY_AGRAVE XK_agrave
    #define PB_KEY_AACUTE XK_aacute
    #define PB_KEY_ACIRCUMFLEX XK_acircumflex
    #define PB_KEY_ATILDE XK_atilde
    #define PB_KEY_ADIAERESIS XK_adiaeresis
    #define PB_KEY_ARING XK_aring
    #define PB_KEY_AE XK_ae
    #define PB_KEY_CCEDILLA XK_ccedilla
    #define PB_KEY_EGRAVE XK_egrave
    #define PB_KEY_EACUTE XK_eacute
    #define PB_KEY_ECIRCUMFLEX XK_ecircumflex
    #define PB_KEY_EDIAERESIS XK_ediaeresis
    #define PB_KEY_IGRAVE XK_igrave
    #define PB_KEY_IACUTE XK_iacute
    #define PB_KEY_ICIRCUMFLEX XK_icircumflex
    #define PB_KEY_IDIAERESIS XK_idiaeresis
    #define PB_KEY_ETH XK_eth
    #define PB_KEY_NTILDE XK_ntilde
    #define PB_KEY_OGRAVE XK_ograve
    #define PB_KEY_OACUTE XK_oacute
    #define PB_KEY_OCIRCUMFLEX XK_ocircumflex
    #define PB_KEY_OTILDE XK_otilde
    #define PB_KEY_ODIAERESIS XK_odiaeresis
    #define PB_KEY_DIVISION XK_division
    #define PB_KEY_OSLASH XK_oslash
    #define PB_KEY_OOBLIQUE XK_ooblique
    #define PB_KEY_UGRAVE XK_ugrave
    #define PB_KEY_UACUTE XK_uacute
    #define PB_KEY_UCIRCUMFLEX XK_ucircumflex
    #define PB_KEY_UDIAERESIS XK_udiaeresis
    #define PB_KEY_YACUTE XK_yacute
    #define PB_KEY_THORN XK_thorn
    #define PB_KEY_YDIAERESIS XK_ydiaeresis
    #endif

    /*
    *LATINLatin
    *BYTEByte
    */

    #ifdef XK_LATIN2
    #define PB_KEY_CAPITAL_AOGONEK XK_Aogonek
    #define PB_KEY_BREVE XK_breve
    #define PB_KEY_CAPITAL_LSTROKE XK_Lstroke
    #define PB_KEY_CAPITAL_LCARON XK_Lcaron
    #define PB_KEY_CAPITAL_SACUTE XK_Sacute
    #define PB_KEY_CAPITAL_SCARON XK_Scaron
    #define PB_KEY_CAPITAL_SCEDILLA XK_Scedilla
    #define PB_KEY_CAPITAL_TCARON XK_Tcaron
    #define PB_KEY_CAPITAL_ZACUTE XK_Zacute
    #define PB_KEY_CAPITAL_ZCARON XK_Zcaron
    #define PB_KEY_CAPITAL_ZABOVEDOT XK_Zabovedot
    #define PB_KEY_AOGONEK XK_aogonek
    #define PB_KEY_OGONEK XK_ogonek
    #define PB_KEY_LSTROKE XK_lstroke
    #define PB_KEY_LCARON XK_lcaron
    #define PB_KEY_SACUTE XK_sacute
    #define PB_KEY_CARON XK_caron
    #define PB_KEY_SCARON XK_scaron
    #define PB_KEY_SCEDILLA XK_scedilla
    #define PB_KEY_TCARON XK_tcaron
    #define PB_KEY_ZACUTE XK_zacute
    #define PB_KEY_DOUBLEACUTE XK_doubleacute
    #define PB_KEY_ZCARON XK_zcaron
    #define PB_KEY_ZABOVEDOT XK_zabovedot
    #define PB_KEY_CAPITAL_RACUTE XK_Racute
    #define PB_KEY_CAPITAL_ABREVE XK_Abreve
    #define PB_KEY_CAPITAL_LACUTE XK_Lacute
    #define PB_KEY_CAPITAL_CACUTE XK_Cacute
    #define PB_KEY_CAPITAL_CCARON XK_Ccaron
    #define PB_KEY_CAPITAL_EOGONEK XK_Eogonek
    #define PB_KEY_CAPITAL_ECARON XK_Ecaron
    #define PB_KEY_CAPITAL_DCARON XK_Dcaron
    #define PB_KEY_CAPITAL_DSTROKE XK_Dstroke
    #define PB_KEY_CAPITAL_NACUTE XK_Nacute
    #define PB_KEY_CAPITAL_NCARON XK_Ncaron
    #define PB_KEY_CAPITAL_ODOUBLEACUTE XK_Odoubleacute
    #define PB_KEY_CAPITAL_RCARON XK_Rcaron
    #define PB_KEY_CAPITAL_URING XK_Uring
    #define PB_KEY_CAPITAL_CAPITAL_UDOUBLEACUTE XK_Udoubleacute
    #define PB_KEY_CAPITAL_CAPITAL_TCEDILLA XK_Tcedilla
    #define PB_KEY_RACUTE XK_racute
    #define PB_KEY_ABREVE XK_abreve
    #define PB_KEY_LACUTE XK_lacute
    #define PB_KEY_CACUTE XK_cacute
    #define PB_KEY_CCARON XK_ccaron
    #define PB_KEY_EOGONEK XK_eogonek
    #define PB_KEY_ECARON XK_ecaron
    #define PB_KEY_DCARON XK_dcaron
    #define PB_KEY_DSTROKE XK_dstroke
    #define PB_KEY_NACUTE XK_nacute
    #define PB_KEY_NCARON XK_ncaron
    #define PB_KEY_ODOUBLEACUTE XK_odoubleacute
    #define PB_KEY_RCARON XK_rcaron
    #define PB_KEY_URING XK_uring
    #define PB_KEY_UDOUBLEACUTE XK_udoubleacute
    #define PB_KEY_TCEDILLA XK_tcedilla
    #define PB_KEY_ABOVEDOT XK_abovedot
    #endif

    /*
    *LATINLatin
    *BYTEByte
    */

    #ifdef XK_LATIN3
    #define PB_KEY_CAPITAL_HSTROKE XK_Hstroke
    #define PB_KEY_CAPITAL_HCIRCUMFLEX XK_Hcircumflex
    #define PB_KEY_CAPITAL_IABOVEDOT XK_Iabovedot
    #define PB_KEY_CAPITAL_GBREVE XK_Gbreve
    #define PB_KEY_CAPITAL_JCIRCUMFLEX XK_Jcircumflex
    #define PB_KEY_HSTROKE XK_hstroke
    #define PB_KEY_HCIRCUMFLEX XK_hcircumflex
    #define PB_KEY_IDOTLESS XK_idotless
    #define PB_KEY_GBREVE XK_gbreve
    #define PB_KEY_JCIRCUMFLEX XK_jcircumflex
    #define PB_KEY_CAPITAL_CABOVEDOT XK_Cabovedot
    #define PB_KEY_CAPITAL_CCIRCUMFLEX XK_Ccircumflex
    #define PB_KEY_CAPITAL_GABOVEDOT XK_Gabovedot
    #define PB_KEY_CAPITAL_GCIRCUMFLEX XK_Gcircumflex
    #define PB_KEY_CAPITAL_UBREVE XK_Ubreve
    #define PB_KEY_CAPITAL_CAPITAL_SCIRCUMFLEX XK_Scircumflex
    #define PB_KEY_CABOVEDOT XK_cabovedot
    #define PB_KEY_CCIRCUMFLEX XK_ccircumflex
    #define PB_KEY_GABOVEDOT XK_gabovedot
    #define PB_KEY_GCIRCUMFLEX XK_gcircumflex
    #define PB_KEY_UBREVE XK_ubreve
    #define PB_KEY_SCIRCUMFLEX XK_scircumflex
    #endif


    /*
    *LATINLatin
    *BYTEByte
    */

    #ifdef XK_LATIN4
    #define PB_KEY_KRA XK_kra
    #define PB_KEY_KAPPA XK_kappa
    #define PB_KEY_CAPITAL_RCEDILLA XK_Rcedilla
    #define PB_KEY_CAPITAL_ITILDE XK_Itilde
    #define PB_KEY_CAPITAL_LCEDILLA XK_Lcedilla
    #define PB_KEY_CAPITAL_EMACRON XK_Emacron
    #define PB_KEY_CAPITAL_GCEDILLA XK_Gcedilla
    #define PB_KEY_CAPITAL_TSLASH XK_Tslash
    #define PB_KEY_RCEDILLA XK_rcedilla
    #define PB_KEY_ITILDE XK_itilde
    #define PB_KEY_LCEDILLA XK_lcedilla
    #define PB_KEY_EMACRON XK_emacron
    #define PB_KEY_GCEDILLA XK_gcedilla
    #define PB_KEY_TSLASH XK_tslash
    #define PB_KEY_CAPITAL_ENG XK_ENG
    #define PB_KEY_ENG XK_eng
    #define PB_KEY_CAPITAL_AMACRON XK_Amacron
    #define PB_KEY_CAPITAL_IOGONEK XK_Iogonek
    #define PB_KEY_CAPITAL_EABOVEDOT XK_Eabovedot
    #define PB_KEY_CAPITAL_IMACRON XK_Imacron
    #define PB_KEY_CAPITAL_NCEDILLA XK_Ncedilla
    #define PB_KEY_CAPITAL_OMACRON XK_Omacron
    #define PB_KEY_CAPITAL_KCEDILLA XK_Kcedilla
    #define PB_KEY_CAPITAL_UOGONEK XK_Uogonek
    #define PB_KEY_CAPITAL_UTILDE XK_Utilde
    #define PB_KEY_CAPITAL_UMACRON XK_Umacron
    #define PB_KEY_AMACRON XK_amacron
    #define PB_KEY_IOGONEK XK_iogonek
    #define PB_KEY_EABOVEDOT XK_eabovedot
    #define PB_KEY_IMACRON XK_imacron
    #define PB_KEY_NCEDILLA XK_ncedilla
    #define PB_KEY_OMACRON XK_omacron
    #define PB_KEY_KCEDILLA XK_kcedilla
    #define PB_KEY_UOGONEK XK_uogonek
    #define PB_KEY_UTILDE XK_utilde
    #define PB_KEY_UMACRON XK_umacron
    #endif

    /*
    *LATINLatin
    */
    #ifdef XK_LATIN8
    #define PB_KEY_CAPITAL_WCIRCUMFLEX XK_Wcircumflex
    #define PB_KEY_WCIRCUMFLEX XK_wcircumflex
    #define PB_KEY_CAPITAL_YCIRCUMFLEX XK_Ycircumflex
    #define PB_KEY_YCIRCUMFLEX XK_ycircumflex
    #define PB_KEY_CAPITAL_BABOVEDOT XK_Babovedot
    #define PB_KEY_BABOVEDOT XK_babovedot
    #define PB_KEY_CAPITAL_DABOVEDOT XK_Dabovedot
    #define PB_KEY_DABOVEDOT XK_dabovedot
    #define PB_KEY_CAPITAL_FABOVEDOT XK_Fabovedot
    #define PB_KEY_FABOVEDOT XK_fabovedot
    #define PB_KEY_CAPITAL_MABOVEDOT XK_Mabovedot
    #define PB_KEY_MABOVEDOT XK_mabovedot
    #define PB_KEY_CAPITAL_PABOVEDOT XK_Pabovedot
    #define PB_KEY_PABOVEDOT XK_pabovedot
    #define PB_KEY_CAPITAL_SABOVEDOT XK_Sabovedot
    #define PB_KEY_SABOVEDOT XK_sabovedot
    #define PB_KEY_CAPITAL_TABOVEDOT XK_Tabovedot
    #define PB_KEY_TABOVEDOT XK_tabovedot
    #define PB_KEY_CAPITAL_WGRAVE XK_Wgrave
    #define PB_KEY_WGRAVE XK_wgrave
    #define PB_KEY_CAPITAL_WACUTE XK_Wacute
    #define PB_KEY_WACUTE XK_wacute
    #define PB_KEY_CAPITAL_WDIAERESIS XK_Wdiaeresis
    #define PB_KEY_WDIAERESIS XK_wdiaeresis
    #define PB_KEY_CAPITAL_YGRAVE XK_Ygrave
    #define PB_KEY_YGRAVE XK_ygrave
    #endif

    /*
    *LATINLatin
    *BYTEByte
    */

    #ifdef XK_LATIN9
    #define PB_KEY_CAPITAL_OE XK_OE
    #define PB_KEY_OE XK_oe
    #define PB_KEY_CAPITAL_YDIAERESIS XK_Ydiaeresis
    #endif

    /*
    *KATAKANAKatakana
    *BYTEByte
    */

    #ifdef XK_KATAKANA
    #define PB_KEY_OVERLINE XK_overline
    #define PB_KEY_KANA_FULLSTOP XK_kana_fullstop
    #define PB_KEY_KANA_OPENINGBRACKET XK_kana_openingbracket
    #define PB_KEY_KANA_CLOSINGBRACKET XK_kana_closingbracket
    #define PB_KEY_KANA_COMMA XK_kana_comma
    #define PB_KEY_KANA_CONJUNCTIVE XK_kana_conjunctive
    #define PB_KEY_KANA_MIDDLEDOT XK_kana_middledot
    #define PB_KEY_KANA_WO XK_kana_WO
    #define PB_KEY_SMALL_KANA_A XK_kana_a
    #define PB_KEY_SMALL_KANA_I XK_kana_i
    #define PB_KEY_SMALL_KANA_U XK_kana_u
    #define PB_KEY_SMALL_KANA_E XK_kana_e
    #define PB_KEY_SMALL_KANA_O XK_kana_o
    #define PB_KEY_SMALL_KANA_YA XK_kana_ya
    #define PB_KEY_SMALL_KANA_YU XK_kana_yu
    #define PB_KEY_SMALL_KANA_YO XK_kana_yo
    #define PB_KEY_SMALL_KANA_TSU XK_kana_tsu
    #define PB_KEY_SMALL_KANA_TU XK_kana_tu
    #define PB_KEY_PROLONGEDSOUND XK_prolongedsound
    #define PB_KEY_KANA_A XK_kana_A
    #define PB_KEY_KANA_I XK_kana_I
    #define PB_KEY_KANA_U XK_kana_U
    #define PB_KEY_KANA_E XK_kana_E
    #define PB_KEY_KANA_O XK_kana_O
    #define PB_KEY_KANA_KA XK_kana_KA
    #define PB_KEY_KANA_KI XK_kana_KI
    #define PB_KEY_KANA_KU XK_kana_KU
    #define PB_KEY_KANA_KE XK_kana_KE
    #define PB_KEY_KANA_KO XK_kana_KO
    #define PB_KEY_KANA_SA XK_kana_SA
    #define PB_KEY_KANA_SHI XK_kana_SHI
    #define PB_KEY_KANA_SU XK_kana_SU
    #define PB_KEY_KANA_SE XK_kana_SE
    #define PB_KEY_KANA_SO XK_kana_SO
    #define PB_KEY_KANA_TA XK_kana_TA
    #define PB_KEY_KANA_CHI XK_kana_CHI
    #define PB_KEY_KANA_TI XK_kana_TI
    #define PB_KEY_KANA_TSU XK_kana_TSU
    #define PB_KEY_KANA_TU XK_kana_TU
    #define PB_KEY_KANA_TE XK_kana_TE
    #define PB_KEY_KANA_TO XK_kana_TO
    #define PB_KEY_KANA_NA XK_kana_NA
    #define PB_KEY_KANA_NI XK_kana_NI
    #define PB_KEY_KANA_NU XK_kana_NU
    #define PB_KEY_KANA_NE XK_kana_NE
    #define PB_KEY_KANA_NO XK_kana_NO
    #define PB_KEY_KANA_HA XK_kana_HA
    #define PB_KEY_KANA_HI XK_kana_HI
    #define PB_KEY_KANA_FU XK_kana_FU
    #define PB_KEY_KANA_HU XK_kana_HU
    #define PB_KEY_KANA_HE XK_kana_HE
    #define PB_KEY_KANA_HO XK_kana_HO
    #define PB_KEY_KANA_MA XK_kana_MA
    #define PB_KEY_KANA_MI XK_kana_MI
    #define PB_KEY_KANA_MU XK_kana_MU
    #define PB_KEY_KANA_ME XK_kana_ME
    #define PB_KEY_KANA_MO XK_kana_MO
    #define PB_KEY_KANA_YA XK_kana_YA
    #define PB_KEY_KANA_YU XK_kana_YU
    #define PB_KEY_KANA_YO XK_kana_YO
    #define PB_KEY_KANA_RA XK_kana_RA
    #define PB_KEY_KANA_RI XK_kana_RI
    #define PB_KEY_KANA_RU XK_kana_RU
    #define PB_KEY_KANA_RE XK_kana_RE
    #define PB_KEY_KANA_RO XK_kana_RO
    #define PB_KEY_KANA_WA XK_kana_WA
    #define PB_KEY_KANA_N XK_kana_N
    #define PB_KEY_VOICEDSOUND XK_voicedsound
    #define PB_KEY_SEMIVOICEDSOUND XK_semivoicedsound
    #define PB_KEY_KANA_SWITCH XK_kana_switch
    #endif

    /*
    *ARABICArabic
    *BYTEByte
    */

    #ifdef XK_ARABIC
    #define PB_KEY_FARSI_0 XK_Farsi_0
    #define PB_KEY_FARSI_1 XK_Farsi_1
    #define PB_KEY_FARSI_2 XK_Farsi_2
    #define PB_KEY_FARSI_3 XK_Farsi_3
    #define PB_KEY_FARSI_4 XK_Farsi_4
    #define PB_KEY_FARSI_5 XK_Farsi_5
    #define PB_KEY_FARSI_6 XK_Farsi_6
    #define PB_KEY_FARSI_7 XK_Farsi_7
    #define PB_KEY_FARSI_8 XK_Farsi_8
    #define PB_KEY_FARSI_9 XK_Farsi_9
    #define PB_KEY_ARABIC_PERCENT XK_Arabic_percent
    #define PB_KEY_ARABIC_SUPERSCRIPT_ALEF XK_Arabic_superscript_alef
    #define PB_KEY_ARABIC_TTEH XK_Arabic_tteh
    #define PB_KEY_ARABIC_PEH XK_Arabic_peh
    #define PB_KEY_ARABIC_TCHEH XK_Arabic_tcheh
    #define PB_KEY_ARABIC_DDAL XK_Arabic_ddal
    #define PB_KEY_ARABIC_RREH XK_Arabic_rreh
    #define PB_KEY_ARABIC_COMMA XK_Arabic_comma
    #define PB_KEY_ARABIC_FULLSTOP XK_Arabic_fullstop
    #define PB_KEY_ARABIC_0 XK_Arabic_0
    #define PB_KEY_ARABIC_1 XK_Arabic_1
    #define PB_KEY_ARABIC_2 XK_Arabic_2
    #define PB_KEY_ARABIC_3 XK_Arabic_3
    #define PB_KEY_ARABIC_4 XK_Arabic_4
    #define PB_KEY_ARABIC_5 XK_Arabic_5
    #define PB_KEY_ARABIC_6 XK_Arabic_6
    #define PB_KEY_ARABIC_7 XK_Arabic_7
    #define PB_KEY_ARABIC_8 XK_Arabic_8
    #define PB_KEY_ARABIC_9 XK_Arabic_9
    #define PB_KEY_ARABIC_SEMICOLON XK_Arabic_semicolon
    #define PB_KEY_ARABIC_QUESTION_MARK XK_Arabic_question_mark
    #define PB_KEY_ARABIC_HAMZA XK_Arabic_hamza
    #define PB_KEY_ARABIC_MADDAONALEF XK_Arabic_maddaonalef
    #define PB_KEY_ARABIC_HAMZAONALEF XK_Arabic_hamzaonalef
    #define PB_KEY_ARABIC_HAMZAONWAW XK_Arabic_hamzaonwaw
    #define PB_KEY_ARABIC_HAMZAUNDERALEF XK_Arabic_hamzaunderalef
    #define PB_KEY_ARABIC_HAMZAONYEH XK_Arabic_hamzaonyeh
    #define PB_KEY_ARABIC_ALEF XK_Arabic_alef
    #define PB_KEY_ARABIC_BEH XK_Arabic_beh
    #define PB_KEY_ARABIC_TEHMARBUTA XK_Arabic_tehmarbuta
    #define PB_KEY_ARABIC_TEH XK_Arabic_teh
    #define PB_KEY_ARABIC_THEH XK_Arabic_theh
    #define PB_KEY_ARABIC_JEEM XK_Arabic_jeem
    #define PB_KEY_ARABIC_HAH XK_Arabic_hah
    #define PB_KEY_ARABIC_KHAH XK_Arabic_khah
    #define PB_KEY_ARABIC_DAL XK_Arabic_dal
    #define PB_KEY_ARABIC_THAL XK_Arabic_thal
    #define PB_KEY_ARABIC_RA XK_Arabic_ra
    #define PB_KEY_ARABIC_ZAIN XK_Arabic_zain
    #define PB_KEY_ARABIC_SEEN XK_Arabic_seen
    #define PB_KEY_ARABIC_SHEEN XK_Arabic_sheen
    #define PB_KEY_ARABIC_SAD XK_Arabic_sad
    #define PB_KEY_ARABIC_DAD XK_Arabic_dad
    #define PB_KEY_ARABIC_TAH XK_Arabic_tah
    #define PB_KEY_ARABIC_ZAH XK_Arabic_zah
    #define PB_KEY_ARABIC_AIN XK_Arabic_ain
    #define PB_KEY_ARABIC_GHAIN XK_Arabic_ghain
    #define PB_KEY_ARABIC_TATWEEL XK_Arabic_tatweel
    #define PB_KEY_ARABIC_FEH XK_Arabic_feh
    #define PB_KEY_ARABIC_QAF XK_Arabic_qaf
    #define PB_KEY_ARABIC_KAF XK_Arabic_kaf
    #define PB_KEY_ARABIC_LAM XK_Arabic_lam
    #define PB_KEY_ARABIC_MEEM XK_Arabic_meem
    #define PB_KEY_ARABIC_NOON XK_Arabic_noon
    #define PB_KEY_ARABIC_HA XK_Arabic_ha
    #define PB_KEY_ARABIC_HEH XK_Arabic_heh
    #define PB_KEY_ARABIC_WAW XK_Arabic_waw
    #define PB_KEY_ARABIC_ALEFMAKSURA XK_Arabic_alefmaksura
    #define PB_KEY_ARABIC_YEH XK_Arabic_yeh
    #define PB_KEY_ARABIC_FATHATAN XK_Arabic_fathatan
    #define PB_KEY_ARABIC_DAMMATAN XK_Arabic_dammatan
    #define PB_KEY_ARABIC_KASRATAN XK_Arabic_kasratan
    #define PB_KEY_ARABIC_FATHA XK_Arabic_fatha
    #define PB_KEY_ARABIC_DAMMA XK_Arabic_damma
    #define PB_KEY_ARABIC_KASRA XK_Arabic_kasra
    #define PB_KEY_ARABIC_SHADDA XK_Arabic_shadda
    #define PB_KEY_ARABIC_SUKUN XK_Arabic_sukun
    #define PB_KEY_ARABIC_MADDA_ABOVE XK_Arabic_madda_above
    #define PB_KEY_ARABIC_HAMZA_ABOVE XK_Arabic_hamza_above
    #define PB_KEY_ARABIC_HAMZA_BELOW XK_Arabic_hamza_below
    #define PB_KEY_ARABIC_JEH XK_Arabic_jeh
    #define PB_KEY_ARABIC_VEH XK_Arabic_veh
    #define PB_KEY_ARABIC_KEHEH XK_Arabic_keheh
    #define PB_KEY_ARABIC_GAF XK_Arabic_gaf
    #define PB_KEY_ARABIC_NOON_GHUNNA XK_Arabic_noon_ghunna
    #define PB_KEY_ARABIC_HEH_DOACHASHMEE XK_Arabic_heh_doachashmee
    #define PB_KEY_FARSI_YEH XK_Farsi_yeh
    #define PB_KEY_ARABIC_FARSI_YEH XK_Arabic_farsi_yeh
    #define PB_KEY_ARABIC_YEH_BAREE XK_Arabic_yeh_baree
    #define PB_KEY_ARABIC_HEH_GOAL XK_Arabic_heh_goal
    #define PB_KEY_ARABIC_SWITCH XK_Arabic_switch
    #endif

    /*
    *CYRILLICCyrillic
    *BYTEByte
    */
    #ifdef XK_CYRILLIC
    #define PB_KEY_CAPITAL_CYRILLIC_GHE_BAR XK_Cyrillic_GHE_bar
    #define PB_KEY_CYRILLIC_GHE_BAR XK_Cyrillic_ghe_bar
    #define PB_KEY_CAPITAL_CYRILLIC_ZHE_DESCENDER XK_Cyrillic_ZHE_descender
    #define PB_KEY_CYRILLIC_ZHE_DESCENDER XK_Cyrillic_zhe_descender
    #define PB_KEY_CAPITAL_CYRILLIC_KA_DESCENDER XK_Cyrillic_KA_descender
    #define PB_KEY_CYRILLIC_KA_DESCENDER XK_Cyrillic_ka_descender
    #define PB_KEY_CAPITAL_CYRILLIC_KA_VERTSTROKE XK_Cyrillic_KA_vertstroke
    #define PB_KEY_CYRILLIC_KA_VERTSTROKE XK_Cyrillic_ka_vertstroke
    #define PB_KEY_CAPITAL_CYRILLIC_EN_DESCENDER XK_Cyrillic_EN_descender
    #define PB_KEY_CYRILLIC_EN_DESCENDER XK_Cyrillic_en_descender
    #define PB_KEY_CAPITAL_CYRILLIC_U_STRAIGHT XK_Cyrillic_U_straight
    #define PB_KEY_CYRILLIC_U_STRAIGHT XK_Cyrillic_u_straight
    #define PB_KEY_CAPITAL_CYRILLIC_U_STRAIGHT_BAR XK_Cyrillic_U_straight_bar
    #define PB_KEY_CYRILLIC_U_STRAIGHT_BAR XK_Cyrillic_u_straight_bar
    #define PB_KEY_CAPITAL_CYRILLIC_HA_DESCENDER XK_Cyrillic_HA_descender
    #define PB_KEY_CYRILLIC_HA_DESCENDER XK_Cyrillic_ha_descender
    #define PB_KEY_CAPITAL_CYRILLIC_CHE_DESCENDER XK_Cyrillic_CHE_descender
    #define PB_KEY_CYRILLIC_CHE_DESCENDER XK_Cyrillic_che_descender
    #define PB_KEY_CAPITAL_CYRILLIC_CHE_VERTSTROKE XK_Cyrillic_CHE_vertstroke
    #define PB_KEY_CYRILLIC_CHE_VERTSTROKE XK_Cyrillic_che_vertstroke
    #define PB_KEY_CAPITAL_CYRILLIC_SHHA XK_Cyrillic_SHHA
    #define PB_KEY_CYRILLIC_SHHA XK_Cyrillic_shha

    #define PB_KEY_CAPITAL_CYRILLIC_SCHWA XK_Cyrillic_SCHWA
    #define PB_KEY_CYRILLIC_SCHWA XK_Cyrillic_schwa
    #define PB_KEY_CAPITAL_CYRILLIC_I_MACRON XK_Cyrillic_I_macron
    #define PB_KEY_CYRILLIC_I_MACRON XK_Cyrillic_i_macron
    #define PB_KEY_CAPITAL_CYRILLIC_O_BAR XK_Cyrillic_O_bar
    #define PB_KEY_CYRILLIC_O_BAR XK_Cyrillic_o_bar
    #define PB_KEY_CAPITAL_CYRILLIC_U_MACRON XK_Cyrillic_U_macron
    #define PB_KEY_CYRILLIC_U_MACRON XK_Cyrillic_u_macron

    #define PB_KEY_SERBIAN_DJE XK_Serbian_dje
    #define PB_KEY_MACEDONIA_GJE XK_Macedonia_gje
    #define PB_KEY_CYRILLIC_IO XK_Cyrillic_io
    #define PB_KEY_UKRAINIAN_IE XK_Ukrainian_ie
    #define PB_KEY_UKRANIAN_JE XK_Ukranian_je
    #define PB_KEY_MACEDONIA_DSE XK_Macedonia_dse
    #define PB_KEY_UKRAINIAN_I XK_Ukrainian_i
    #define PB_KEY_UKRANIAN_I XK_Ukranian_i
    #define PB_KEY_UKRAINIAN_YI XK_Ukrainian_yi
    #define PB_KEY_UKRANIAN_YI XK_Ukranian_yi
    #define PB_KEY_CYRILLIC_JE XK_Cyrillic_je
    #define PB_KEY_SERBIAN_JE XK_Serbian_je
    #define PB_KEY_CYRILLIC_LJE XK_Cyrillic_lje
    #define PB_KEY_SERBIAN_LJE XK_Serbian_lje
    #define PB_KEY_CYRILLIC_NJE XK_Cyrillic_nje
    #define PB_KEY_SERBIAN_NJE XK_Serbian_nje
    #define PB_KEY_SERBIAN_TSHE XK_Serbian_tshe
    #define PB_KEY_MACEDONIA_KJE XK_Macedonia_kje
    #define PB_KEY_UKRAINIAN_GHE_WITH_UPTURN XK_Ukrainian_ghe_with_upturn
    #define PB_KEY_BYELORUSSIAN_SHORTU XK_Byelorussian_shortu
    #define PB_KEY_CYRILLIC_DZHE XK_Cyrillic_dzhe
    #define PB_KEY_SERBIAN_DZE XK_Serbian_dze
    #define PB_KEY_NUMEROSIGN XK_numerosign
    #define PB_KEY_CAPITAL_SERBIAN_DJE XK_Serbian_DJE
    #define PB_KEY_CAPITAL_MACEDONIA_GJE XK_Macedonia_GJE
    #define PB_KEY_CAPITAL_CYRILLIC_IO XK_Cyrillic_IO
    #define PB_KEY_CAPITAL_UKRAINIAN_IE XK_Ukrainian_IE
    #define PB_KEY_CAPITAL_UKRANIAN_JE XK_Ukranian_JE
    #define PB_KEY_CAPITAL_MACEDONIA_DSE XK_Macedonia_DSE
    #define PB_KEY_CAPITAL_UKRAINIAN_I XK_Ukrainian_I
    #define PB_KEY_CAPITAL_UKRANIAN_I XK_Ukranian_I
    #define PB_KEY_CAPITAL_UKRAINIAN_YI XK_Ukrainian_YI
    #define PB_KEY_CAPITAL_UKRANIAN_YI XK_Ukranian_YI
    #define PB_KEY_CAPITAL_CYRILLIC_JE XK_Cyrillic_JE
    #define PB_KEY_CAPITAL_SERBIAN_JE XK_Serbian_JE
    #define PB_KEY_CAPITAL_CYRILLIC_LJE XK_Cyrillic_LJE
    #define PB_KEY_CAPITAL_SERBIAN_LJE XK_Serbian_LJE
    #define PB_KEY_CAPITAL_CYRILLIC_NJE XK_Cyrillic_NJE
    #define PB_KEY_CAPITAL_SERBIAN_NJE XK_Serbian_NJE
    #define PB_KEY_CAPITAL_SERBIAN_TSHE XK_Serbian_TSHE
    #define PB_KEY_CAPITAL_MACEDONIA_KJE XK_Macedonia_KJE
    #define PB_KEY_CAPITAL_UKRAINIAN_GHE_WITH_UPTURN XK_Ukrainian_GHE_WITH_UPTURN
    #define PB_KEY_CAPITAL_BYELORUSSIAN_SHORTU XK_Byelorussian_SHORTU
    #define PB_KEY_CAPITAL_CYRILLIC_DZHE XK_Cyrillic_DZHE
    #define PB_KEY_CAPITAL_SERBIAN_DZE XK_Serbian_DZE
    #define PB_KEY_CYRILLIC_YU XK_Cyrillic_yu
    #define PB_KEY_CYRILLIC_A XK_Cyrillic_a
    #define PB_KEY_CYRILLIC_BE XK_Cyrillic_be
    #define PB_KEY_CYRILLIC_TSE XK_Cyrillic_tse
    #define PB_KEY_CYRILLIC_DE XK_Cyrillic_de
    #define PB_KEY_CYRILLIC_IE XK_Cyrillic_ie
    #define PB_KEY_CYRILLIC_EF XK_Cyrillic_ef
    #define PB_KEY_CYRILLIC_GHE XK_Cyrillic_ghe
    #define PB_KEY_CYRILLIC_HA XK_Cyrillic_ha
    #define PB_KEY_CYRILLIC_I XK_Cyrillic_i
    #define PB_KEY_CYRILLIC_SHORTI XK_Cyrillic_shorti
    #define PB_KEY_CYRILLIC_KA XK_Cyrillic_ka
    #define PB_KEY_CYRILLIC_EL XK_Cyrillic_el
    #define PB_KEY_CYRILLIC_EM XK_Cyrillic_em
    #define PB_KEY_CYRILLIC_EN XK_Cyrillic_en
    #define PB_KEY_CYRILLIC_O XK_Cyrillic_o
    #define PB_KEY_CYRILLIC_PE XK_Cyrillic_pe
    #define PB_KEY_CYRILLIC_YA XK_Cyrillic_ya
    #define PB_KEY_CYRILLIC_ER XK_Cyrillic_er
    #define PB_KEY_CYRILLIC_ES XK_Cyrillic_es
    #define PB_KEY_CYRILLIC_TE XK_Cyrillic_te
    #define PB_KEY_CYRILLIC_U XK_Cyrillic_u
    #define PB_KEY_CYRILLIC_ZHE XK_Cyrillic_zhe
    #define PB_KEY_CYRILLIC_VE XK_Cyrillic_ve
    #define PB_KEY_CYRILLIC_SOFTSIGN XK_Cyrillic_softsign
    #define PB_KEY_CYRILLIC_YERU XK_Cyrillic_yeru
    #define PB_KEY_CYRILLIC_ZE XK_Cyrillic_ze
    #define PB_KEY_CYRILLIC_SHA XK_Cyrillic_sha
    #define PB_KEY_CYRILLIC_E XK_Cyrillic_e
    #define PB_KEY_CYRILLIC_SHCHA XK_Cyrillic_shcha
    #define PB_KEY_CYRILLIC_CHE XK_Cyrillic_che
    #define PB_KEY_CYRILLIC_HARDSIGN XK_Cyrillic_hardsign
    #define PB_KEY_CAPITAL_CYRILLIC_YU XK_Cyrillic_YU
    #define PB_KEY_CAPITAL_CYRILLIC_A XK_Cyrillic_A
    #define PB_KEY_CAPITAL_CYRILLIC_BE XK_Cyrillic_BE
    #define PB_KEY_CAPITAL_CYRILLIC_TSE XK_Cyrillic_TSE
    #define PB_KEY_CAPITAL_CYRILLIC_DE XK_Cyrillic_DE
    #define PB_KEY_CAPITAL_CYRILLIC_IE XK_Cyrillic_IE
    #define PB_KEY_CAPITAL_CYRILLIC_EF XK_Cyrillic_EF
    #define PB_KEY_CAPITAL_CYRILLIC_GHE XK_Cyrillic_GHE
    #define PB_KEY_CAPITAL_CYRILLIC_HA XK_Cyrillic_HA
    #define PB_KEY_CAPITAL_CYRILLIC_I XK_Cyrillic_I
    #define PB_KEY_CAPITAL_CYRILLIC_SHORTI XK_Cyrillic_SHORTI
    #define PB_KEY_CAPITAL_CYRILLIC_KA XK_Cyrillic_KA
    #define PB_KEY_CAPITAL_CYRILLIC_EL XK_Cyrillic_EL
    #define PB_KEY_CAPITAL_CYRILLIC_EM XK_Cyrillic_EM
    #define PB_KEY_CAPITAL_CYRILLIC_EN XK_Cyrillic_EN
    #define PB_KEY_CAPITAL_CYRILLIC_O XK_Cyrillic_O
    #define PB_KEY_CAPITAL_CYRILLIC_PE XK_Cyrillic_PE
    #define PB_KEY_CAPITAL_CYRILLIC_YA XK_Cyrillic_YA
    #define PB_KEY_CAPITAL_CYRILLIC_ER XK_Cyrillic_ER
    #define PB_KEY_CAPITAL_CYRILLIC_ES XK_Cyrillic_ES
    #define PB_KEY_CAPITAL_CYRILLIC_TE XK_Cyrillic_TE
    #define PB_KEY_CAPITAL_CYRILLIC_U XK_Cyrillic_U
    #define PB_KEY_CAPITAL_CYRILLIC_ZHE XK_Cyrillic_ZHE
    #define PB_KEY_CAPITAL_CYRILLIC_VE XK_Cyrillic_VE
    #define PB_KEY_CAPITAL_CYRILLIC_SOFTSIGN XK_Cyrillic_SOFTSIGN
    #define PB_KEY_CAPITAL_CYRILLIC_YERU XK_Cyrillic_YERU
    #define PB_KEY_CAPITAL_CYRILLIC_ZE XK_Cyrillic_ZE
    #define PB_KEY_CAPITAL_CYRILLIC_SHA XK_Cyrillic_SHA
    #define PB_KEY_CAPITAL_CYRILLIC_E XK_Cyrillic_E
    #define PB_KEY_CAPITAL_CYRILLIC_SHCHA XK_Cyrillic_SHCHA
    #define PB_KEY_CAPITAL_CYRILLIC_CHE XK_Cyrillic_CHE
    #define PB_KEY_CAPITAL_CYRILLIC_HARDSIGN XK_Cyrillic_HARDSIGN
    #endif

    /* * Greek * (based on an early draft of, and not quite identical to, ISO/IEC 8859-7) * Byte 3 = 7          */              





    #ifdef XK_GREEK
    #define PB_KEY_CAPITAL_GREEK_ALPHAACCENT XK_Greek_ALPHAaccent
    #define PB_KEY_CAPITAL_GREEK_EPSILONACCENT XK_Greek_EPSILONaccent
    #define PB_KEY_CAPITAL_GREEK_ETAACCENT XK_Greek_ETAaccent
    #define PB_KEY_CAPITAL_GREEK_IOTAACCENT XK_Greek_IOTAaccent
    #define PB_KEY_CAPITAL_GREEK_IOTADIERESIS XK_Greek_IOTAdieresis
    #define PB_KEY_CAPITAL_GREEK_IOTADIAERESIS XK_Greek_IOTAdiaeresis
    #define PB_KEY_CAPITAL_GREEK_OMICRONACCENT XK_Greek_OMICRONaccent
    #define PB_KEY_CAPITAL_GREEK_UPSILONACCENT XK_Greek_UPSILONaccent
    #define PB_KEY_CAPITAL_GREEK_UPSILONDIERESIS XK_Greek_UPSILONdieresis
    #define PB_KEY_CAPITAL_GREEK_OMEGAACCENT XK_Greek_OMEGAaccent
    #define PB_KEY_GREEK_ACCENTDIERESIS XK_Greek_accentdieresis
    #define PB_KEY_GREEK_HORIZBAR XK_Greek_horizbar
    #define PB_KEY_GREEK_ALPHAACCENT XK_Greek_alphaaccent
    #define PB_KEY_GREEK_EPSILONACCENT XK_Greek_epsilonaccent
    #define PB_KEY_GREEK_ETAACCENT XK_Greek_etaaccent
    #define PB_KEY_GREEK_IOTAACCENT XK_Greek_iotaaccent
    #define PB_KEY_GREEK_IOTADIERESIS XK_Greek_iotadieresis
    #define PB_KEY_GREEK_IOTAACCENTDIERESIS XK_Greek_iotaaccentdieresis
    #define PB_KEY_GREEK_OMICRONACCENT XK_Greek_omicronaccent
    #define PB_KEY_GREEK_UPSILONACCENT XK_Greek_upsilonaccent
    #define PB_KEY_GREEK_UPSILONDIERESIS XK_Greek_upsilondieresis
    #define PB_KEY_GREEK_UPSILONACCENTDIERESIS XK_Greek_upsilonaccentdieresis
    #define PB_KEY_GREEK_OMEGAACCENT XK_Greek_omegaaccent
    #define PB_KEY_CAPITAL_GREEK_ALPHA XK_Greek_ALPHA
    #define PB_KEY_CAPITAL_GREEK_BETA XK_Greek_BETA
    #define PB_KEY_CAPITAL_GREEK_GAMMA XK_Greek_GAMMA
    #define PB_KEY_CAPITAL_GREEK_DELTA XK_Greek_DELTA
    #define PB_KEY_CAPITAL_GREEK_EPSILON XK_Greek_EPSILON
    #define PB_KEY_CAPITAL_GREEK_ZETA XK_Greek_ZETA
    #define PB_KEY_CAPITAL_GREEK_ETA XK_Greek_ETA
    #define PB_KEY_CAPITAL_GREEK_THETA XK_Greek_THETA
    #define PB_KEY_CAPITAL_GREEK_IOTA XK_Greek_IOTA
    #define PB_KEY_CAPITAL_GREEK_KAPPA XK_Greek_KAPPA
    #define PB_KEY_CAPITAL_GREEK_LAMDA XK_Greek_LAMDA
    #define PB_KEY_CAPITAL_GREEK_LAMBDA XK_Greek_LAMBDA
    #define PB_KEY_CAPITAL_GREEK_MU XK_Greek_MU
    #define PB_KEY_CAPITAL_GREEK_NU XK_Greek_NU
    #define PB_KEY_CAPITAL_GREEK_XI XK_Greek_XI
    #define PB_KEY_CAPITAL_GREEK_OMICRON XK_Greek_OMICRON
    #define PB_KEY_CAPITAL_GREEK_PI XK_Greek_PI
    #define PB_KEY_CAPITAL_GREEK_RHO XK_Greek_RHO
    #define PB_KEY_CAPITAL_GREEK_SIGMA XK_Greek_SIGMA
    #define PB_KEY_CAPITAL_GREEK_TAU XK_Greek_TAU
    #define PB_KEY_CAPITAL_GREEK_UPSILON XK_Greek_UPSILON
    #define PB_KEY_CAPITAL_GREEK_PHI XK_Greek_PHI
    #define PB_KEY_CAPITAL_GREEK_CHI XK_Greek_CHI
    #define PB_KEY_CAPITAL_GREEK_PSI XK_Greek_PSI
    #define PB_KEY_CAPITAL_GREEK_OMEGA XK_Greek_OMEGA
    #define PB_KEY_GREEK_ALPHA XK_Greek_alpha
    #define PB_KEY_GREEK_BETA XK_Greek_beta
    #define PB_KEY_GREEK_GAMMA XK_Greek_gamma
    #define PB_KEY_GREEK_DELTA XK_Greek_delta
    #define PB_KEY_GREEK_EPSILON XK_Greek_epsilon
    #define PB_KEY_GREEK_ZETA XK_Greek_zeta
    #define PB_KEY_GREEK_ETA XK_Greek_eta
    #define PB_KEY_GREEK_THETA XK_Greek_theta
    #define PB_KEY_GREEK_IOTA XK_Greek_iota
    #define PB_KEY_GREEK_KAPPA XK_Greek_kappa
    #define PB_KEY_GREEK_LAMDA XK_Greek_lamda
    #define PB_KEY_GREEK_LAMBDA XK_Greek_lambda
    #define PB_KEY_GREEK_MU XK_Greek_mu
    #define PB_KEY_GREEK_NU XK_Greek_nu
    #define PB_KEY_GREEK_XI XK_Greek_xi
    #define PB_KEY_GREEK_OMICRON XK_Greek_omicron
    #define PB_KEY_GREEK_PI XK_Greek_pi
    #define PB_KEY_GREEK_RHO XK_Greek_rho
    #define PB_KEY_GREEK_SIGMA XK_Greek_sigma
    #define PB_KEY_GREEK_FINALSMALLSIGMA XK_Greek_finalsmallsigma
    #define PB_KEY_GREEK_TAU XK_Greek_tau
    #define PB_KEY_GREEK_UPSILON XK_Greek_upsilon
    #define PB_KEY_GREEK_PHI XK_Greek_phi
    #define PB_KEY_GREEK_CHI XK_Greek_chi
    #define PB_KEY_GREEK_PSI XK_Greek_psi
    #define PB_KEY_GREEK_OMEGA XK_Greek_omega
    #define PB_KEY_GREEK_SWITCH XK_Greek_switch
    #endif

    /* Technical * (from the DEC VT330/VT420 Technical Character Set, http://vt100.net/charsets/technical.html) * Byte 3 = 8     */         





    #ifdef XK_TECHNICAL
    #define PB_KEY_LEFTRADICAL XK_leftradical
    #define PB_KEY_TOPLEFTRADICAL XK_topleftradical
    #define PB_KEY_HORIZCONNECTOR XK_horizconnector
    #define PB_KEY_TOPINTEGRAL XK_topintegral
    #define PB_KEY_BOTINTEGRAL XK_botintegral
    #define PB_KEY_VERTCONNECTOR XK_vertconnector
    #define PB_KEY_TOPLEFTSQBRACKET XK_topleftsqbracket
    #define PB_KEY_BOTLEFTSQBRACKET XK_botleftsqbracket
    #define PB_KEY_TOPRIGHTSQBRACKET XK_toprightsqbracket
    #define PB_KEY_BOTRIGHTSQBRACKET XK_botrightsqbracket
    #define PB_KEY_TOPLEFTPARENS XK_topleftparens
    #define PB_KEY_BOTLEFTPARENS XK_botleftparens
    #define PB_KEY_TOPRIGHTPARENS XK_toprightparens
    #define PB_KEY_BOTRIGHTPARENS XK_botrightparens
    #define PB_KEY_LEFTMIDDLECURLYBRACE XK_leftmiddlecurlybrace
    #define PB_KEY_RIGHTMIDDLECURLYBRACE XK_rightmiddlecurlybrace
    #define PB_KEY_TOPLEFTSUMMATION XK_topleftsummation
    #define PB_KEY_BOTLEFTSUMMATION XK_botleftsummation
    #define PB_KEY_TOPVERTSUMMATIONCONNECTOR XK_topvertsummationconnector
    #define PB_KEY_BOTVERTSUMMATIONCONNECTOR XK_botvertsummationconnector
    #define PB_KEY_TOPRIGHTSUMMATION XK_toprightsummation
    #define PB_KEY_BOTRIGHTSUMMATION XK_botrightsummation
    #define PB_KEY_RIGHTMIDDLESUMMATION XK_rightmiddlesummation
    #define PB_KEY_LESSTHANEQUAL XK_lessthanequal
    #define PB_KEY_NOTEQUAL XK_notequal
    #define PB_KEY_GREATERTHANEQUAL XK_greaterthanequal
    #define PB_KEY_INTEGRAL XK_integral
    #define PB_KEY_THEREFORE XK_therefore
    #define PB_KEY_VARIATION XK_variation
    #define PB_KEY_INFINITY XK_infinity
    #define PB_KEY_NABLA XK_nabla
    #define PB_KEY_APPROXIMATE XK_approximate
    #define PB_KEY_SIMILAREQUAL XK_similarequal
    #define PB_KEY_IFONLYIF XK_ifonlyif
    #define PB_KEY_IMPLIES XK_implies
    #define PB_KEY_IDENTICAL XK_identical
    #define PB_KEY_RADICAL XK_radical
    #define PB_KEY_INCLUDEDIN XK_includedin
    #define PB_KEY_INCLUDES XK_includes
    #define PB_KEY_INTERSECTION XK_intersection
    #define PB_KEY_UNION XK_union
    #define PB_KEY_LOGICALAND XK_logicaland
    #define PB_KEY_LOGICALOR XK_logicalor
    #define PB_KEY_PARTIALDERIVATIVE XK_partialderivative
    #define PB_KEY_FUNCTION XK_function
    #define PB_KEY_LEFTARROW XK_leftarrow
    #define PB_KEY_UPARROW XK_uparrow
    #define PB_KEY_RIGHTARROW XK_rightarrow
    #define PB_KEY_DOWNARROW XK_downarrow
    #endif

    /** Special        * (from the DEC VT100 Special Graphics Character Set) * Byte 3 = 9     */         





    #ifdef XK_SPECIAL
    #define PB_KEY_BLANK XK_blank
    #define PB_KEY_SOLIDDIAMOND XK_soliddiamond
    #define PB_KEY_CHECKERBOARD XK_checkerboard
    #define PB_KEY_HT XK_ht
    #define PB_KEY_FF XK_ff
    #define PB_KEY_CR XK_cr
    #define PB_KEY_LF XK_lf
    #define PB_KEY_NL XK_nl
    #define PB_KEY_VT XK_vt
    #define PB_KEY_LOWRIGHTCORNER XK_lowrightcorner
    #define PB_KEY_UPRIGHTCORNER XK_uprightcorner
    #define PB_KEY_UPLEFTCORNER XK_upleftcorner
    #define PB_KEY_LOWLEFTCORNER XK_lowleftcorner
    #define PB_KEY_CROSSINGLINES XK_crossinglines
    #define PB_KEY_HORIZLINESCAN1 XK_horizlinescan1
    #define PB_KEY_HORIZLINESCAN3 XK_horizlinescan3
    #define PB_KEY_HORIZLINESCAN5 XK_horizlinescan5
    #define PB_KEY_HORIZLINESCAN7 XK_horizlinescan7
    #define PB_KEY_HORIZLINESCAN9 XK_horizlinescan9
    #define PB_KEY_LEFTT XK_leftt
    #define PB_KEY_RIGHTT XK_rightt
    #define PB_KEY_BOTT XK_bott
    #define PB_KEY_TOPT XK_topt
    #define PB_KEY_VERTBAR XK_vertbar
    #endif

    /** Publishing         * (these are probably from a long forgotten DEC Publishing * font that once shipped with DECwrite)    * Byte 3 = 0x0a      */          






    #ifdef XK_PUBLISHING
    #define PB_KEY_EMSPACE XK_emspace
    #define PB_KEY_ENSPACE XK_enspace
    #define PB_KEY_EM3SPACE XK_em3space
    #define PB_KEY_EM4SPACE XK_em4space
    #define PB_KEY_DIGITSPACE XK_digitspace
    #define PB_KEY_PUNCTSPACE XK_punctspace
    #define PB_KEY_THINSPACE XK_thinspace
    #define PB_KEY_HAIRSPACE XK_hairspace
    #define PB_KEY_EMDASH XK_emdash
    #define PB_KEY_ENDASH XK_endash
    #define PB_KEY_SIGNIFBLANK XK_signifblank
    #define PB_KEY_ELLIPSIS XK_ellipsis
    #define PB_KEY_DOUBBASELINEDOT XK_doubbaselinedot
    #define PB_KEY_ONETHIRD XK_onethird
    #define PB_KEY_TWOTHIRDS XK_twothirds
    #define PB_KEY_ONEFIFTH XK_onefifth
    #define PB_KEY_TWOFIFTHS XK_twofifths
    #define PB_KEY_THREEFIFTHS XK_threefifths
    #define PB_KEY_FOURFIFTHS XK_fourfifths
    #define PB_KEY_ONESIXTH XK_onesixth
    #define PB_KEY_FIVESIXTHS XK_fivesixths
    #define PB_KEY_CAREOF XK_careof
    #define PB_KEY_FIGDASH XK_figdash
    #define PB_KEY_LEFTANGLEBRACKET XK_leftanglebracket
    #define PB_KEY_DECIMALPOINT XK_decimalpoint
    #define PB_KEY_RIGHTANGLEBRACKET XK_rightanglebracket
    #define PB_KEY_MARKER XK_marker
    #define PB_KEY_ONEEIGHTH XK_oneeighth
    #define PB_KEY_THREEEIGHTHS XK_threeeighths
    #define PB_KEY_FIVEEIGHTHS XK_fiveeighths
    #define PB_KEY_SEVENEIGHTHS XK_seveneighths
    #define PB_KEY_TRADEMARK XK_trademark
    #define PB_KEY_SIGNATUREMARK XK_signaturemark
    #define PB_KEY_TRADEMARKINCIRCLE XK_trademarkincircle
    #define PB_KEY_LEFTOPENTRIANGLE XK_leftopentriangle
    #define PB_KEY_RIGHTOPENTRIANGLE XK_rightopentriangle
    #define PB_KEY_EMOPENCIRCLE XK_emopencircle
    #define PB_KEY_EMOPENRECTANGLE XK_emopenrectangle
    #define PB_KEY_LEFTSINGLEQUOTEMARK XK_leftsinglequotemark
    #define PB_KEY_RIGHTSINGLEQUOTEMARK XK_rightsinglequotemark
    #define PB_KEY_LEFTDOUBLEQUOTEMARK XK_leftdoublequotemark
    #define PB_KEY_RIGHTDOUBLEQUOTEMARK XK_rightdoublequotemark
    #define PB_KEY_PRESCRIPTION XK_prescription
    #define PB_KEY_PERMILLE XK_permille
    #define PB_KEY_MINUTES XK_minutes
    #define PB_KEY_SECONDS XK_seconds
    #define PB_KEY_LATINCROSS XK_latincross
    #define PB_KEY_HEXAGRAM XK_hexagram
    #define PB_KEY_FILLEDRECTBULLET XK_filledrectbullet
    #define PB_KEY_FILLEDLEFTTRIBULLET XK_filledlefttribullet
    #define PB_KEY_FILLEDRIGHTTRIBULLET XK_filledrighttribullet
    #define PB_KEY_EMFILLEDCIRCLE XK_emfilledcircle
    #define PB_KEY_EMFILLEDRECT XK_emfilledrect
    #define PB_KEY_ENOPENCIRCBULLET XK_enopencircbullet
    #define PB_KEY_ENOPENSQUAREBULLET XK_enopensquarebullet
    #define PB_KEY_OPENRECTBULLET XK_openrectbullet
    #define PB_KEY_OPENTRIBULLETUP XK_opentribulletup
    #define PB_KEY_OPENTRIBULLETDOWN XK_opentribulletdown
    #define PB_KEY_OPENSTAR XK_openstar
    #define PB_KEY_ENFILLEDCIRCBULLET XK_enfilledcircbullet
    #define PB_KEY_ENFILLEDSQBULLET XK_enfilledsqbullet
    #define PB_KEY_FILLEDTRIBULLETUP XK_filledtribulletup
    #define PB_KEY_FILLEDTRIBULLETDOWN XK_filledtribulletdown
    #define PB_KEY_LEFTPOINTER XK_leftpointer
    #define PB_KEY_RIGHTPOINTER XK_rightpointer
    #define PB_KEY_CLUB XK_club
    #define PB_KEY_DIAMOND XK_diamond
    #define PB_KEY_HEART XK_heart
    #define PB_KEY_MALTESECROSS XK_maltesecross
    #define PB_KEY_DAGGER XK_dagger
    #define PB_KEY_DOUBLEDAGGER XK_doubledagger
    #define PB_KEY_CHECKMARK XK_checkmark
    #define PB_KEY_BALLOTCROSS XK_ballotcross
    #define PB_KEY_MUSICALSHARP XK_musicalsharp
    #define PB_KEY_MUSICALFLAT XK_musicalflat
    #define PB_KEY_MALESYMBOL XK_malesymbol
    #define PB_KEY_FEMALESYMBOL XK_femalesymbol
    #define PB_KEY_TELEPHONE XK_telephone
    #define PB_KEY_TELEPHONERECORDER XK_telephonerecorder
    #define PB_KEY_PHONOGRAPHCOPYRIGHT XK_phonographcopyright
    #define PB_KEY_CARET XK_caret
    #define PB_KEY_SINGLELOWQUOTEMARK XK_singlelowquotemark
    #define PB_KEY_DOUBLELOWQUOTEMARK XK_doublelowquotemark
    #define PB_KEY_CURSOR XK_cursor
    #endif

    /*
    *APLAPL
    *BYTEByte
    */

    #ifdef XK_APL
    #define PB_KEY_LEFTCARET XK_leftcaret
    #define PB_KEY_RIGHTCARET XK_rightcaret
    #define PB_KEY_DOWNCARET XK_downcaret
    #define PB_KEY_UPCARET XK_upcaret
    #define PB_KEY_OVERBAR XK_overbar
    #define PB_KEY_DOWNTACK XK_downtack
    #define PB_KEY_UPSHOE XK_upshoe
    #define PB_KEY_DOWNSTILE XK_downstile
    #define PB_KEY_UNDERBAR XK_underbar
    #define PB_KEY_JOT XK_jot
    #define PB_KEY_QUAD XK_quad
    #define PB_KEY_UPTACK XK_uptack
    #define PB_KEY_CIRCLE XK_circle
    #define PB_KEY_UPSTILE XK_upstile
    #define PB_KEY_DOWNSHOE XK_downshoe
    #define PB_KEY_RIGHTSHOE XK_rightshoe
    #define PB_KEY_LEFTSHOE XK_leftshoe
    #define PB_KEY_LEFTTACK XK_lefttack
    #define PB_KEY_RIGHTTACK XK_righttack
    #endif

    /*
    *HEBREWHebrew
    *BYTEByte
    */

    #ifdef XK_HEBREW
    #define PB_KEY_HEBREW_DOUBLELOWLINE XK_hebrew_doublelowline
    #define PB_KEY_HEBREW_ALEPH XK_hebrew_aleph
    #define PB_KEY_HEBREW_BET XK_hebrew_bet
    #define PB_KEY_HEBREW_BETH XK_hebrew_beth
    #define PB_KEY_HEBREW_GIMEL XK_hebrew_gimel
    #define PB_KEY_HEBREW_GIMMEL XK_hebrew_gimmel
    #define PB_KEY_HEBREW_DALET XK_hebrew_dalet
    #define PB_KEY_HEBREW_DALETH XK_hebrew_daleth
    #define PB_KEY_HEBREW_HE XK_hebrew_he
    #define PB_KEY_HEBREW_WAW XK_hebrew_waw
    #define PB_KEY_HEBREW_ZAIN XK_hebrew_zain
    #define PB_KEY_HEBREW_ZAYIN XK_hebrew_zayin
    #define PB_KEY_HEBREW_CHET XK_hebrew_chet
    #define PB_KEY_HEBREW_HET XK_hebrew_het
    #define PB_KEY_HEBREW_TET XK_hebrew_tet
    #define PB_KEY_HEBREW_TETH XK_hebrew_teth
    #define PB_KEY_HEBREW_YOD XK_hebrew_yod
    #define PB_KEY_HEBREW_FINALKAPH XK_hebrew_finalkaph
    #define PB_KEY_HEBREW_KAPH XK_hebrew_kaph
    #define PB_KEY_HEBREW_LAMED XK_hebrew_lamed
    #define PB_KEY_HEBREW_FINALMEM XK_hebrew_finalmem
    #define PB_KEY_HEBREW_MEM XK_hebrew_mem
    #define PB_KEY_HEBREW_FINALNUN XK_hebrew_finalnun
    #define PB_KEY_HEBREW_NUN XK_hebrew_nun
    #define PB_KEY_HEBREW_SAMECH XK_hebrew_samech
    #define PB_KEY_HEBREW_SAMEKH XK_hebrew_samekh
    #define PB_KEY_HEBREW_AYIN XK_hebrew_ayin
    #define PB_KEY_HEBREW_FINALPE XK_hebrew_finalpe
    #define PB_KEY_HEBREW_PE XK_hebrew_pe
    #define PB_KEY_HEBREW_FINALZADE XK_hebrew_finalzade
    #define PB_KEY_HEBREW_FINALZADI XK_hebrew_finalzadi
    #define PB_KEY_HEBREW_ZADE XK_hebrew_zade
    #define PB_KEY_HEBREW_ZADI XK_hebrew_zadi
    #define PB_KEY_HEBREW_QOPH XK_hebrew_qoph
    #define PB_KEY_HEBREW_KUF XK_hebrew_kuf
    #define PB_KEY_HEBREW_RESH XK_hebrew_resh
    #define PB_KEY_HEBREW_SHIN XK_hebrew_shin
    #define PB_KEY_HEBREW_TAW XK_hebrew_taw
    #define PB_KEY_HEBREW_TAF XK_hebrew_taf
    #define PB_KEY_HEBREW_SWITCH XK_Hebrew_switch
    #endif 

    /*
    *THAIThai
    *BYTEByte
    */

    #ifdef XK_THAI
    #define PB_KEY_THAI_KOKAI XK_Thai_kokai
    #define PB_KEY_THAI_KHOKHAI XK_Thai_khokhai
    #define PB_KEY_THAI_KHOKHUAT XK_Thai_khokhuat
    #define PB_KEY_THAI_KHOKHWAI XK_Thai_khokhwai
    #define PB_KEY_THAI_KHOKHON XK_Thai_khokhon
    #define PB_KEY_THAI_KHORAKHANG XK_Thai_khorakhang
    #define PB_KEY_THAI_NGONGU XK_Thai_ngongu
    #define PB_KEY_THAI_CHOCHAN XK_Thai_chochan
    #define PB_KEY_THAI_CHOCHING XK_Thai_choching
    #define PB_KEY_THAI_CHOCHANG XK_Thai_chochang
    #define PB_KEY_THAI_SOSO XK_Thai_soso
    #define PB_KEY_THAI_CHOCHOE XK_Thai_chochoe
    #define PB_KEY_THAI_YOYING XK_Thai_yoying
    #define PB_KEY_THAI_DOCHADA XK_Thai_dochada
    #define PB_KEY_THAI_TOPATAK XK_Thai_topatak
    #define PB_KEY_THAI_THOTHAN XK_Thai_thothan
    #define PB_KEY_THAI_THONANGMONTHO XK_Thai_thonangmontho
    #define PB_KEY_THAI_THOPHUTHAO XK_Thai_thophuthao
    #define PB_KEY_THAI_NONEN XK_Thai_nonen
    #define PB_KEY_THAI_DODEK XK_Thai_dodek
    #define PB_KEY_THAI_TOTAO XK_Thai_totao
    #define PB_KEY_THAI_THOTHUNG XK_Thai_thothung
    #define PB_KEY_THAI_THOTHAHAN XK_Thai_thothahan
    #define PB_KEY_THAI_THOTHONG XK_Thai_thothong
    #define PB_KEY_THAI_NONU XK_Thai_nonu
    #define PB_KEY_THAI_BOBAIMAI XK_Thai_bobaimai
    #define PB_KEY_THAI_POPLA XK_Thai_popla
    #define PB_KEY_THAI_PHOPHUNG XK_Thai_phophung
    #define PB_KEY_THAI_FOFA XK_Thai_fofa
    #define PB_KEY_THAI_PHOPHAN XK_Thai_phophan
    #define PB_KEY_THAI_FOFAN XK_Thai_fofan
    #define PB_KEY_THAI_PHOSAMPHAO XK_Thai_phosamphao
    #define PB_KEY_THAI_MOMA XK_Thai_moma
    #define PB_KEY_THAI_YOYAK XK_Thai_yoyak
    #define PB_KEY_THAI_RORUA XK_Thai_rorua
    #define PB_KEY_THAI_RU XK_Thai_ru
    #define PB_KEY_THAI_LOLING XK_Thai_loling
    #define PB_KEY_THAI_LU XK_Thai_lu
    #define PB_KEY_THAI_WOWAEN XK_Thai_wowaen
    #define PB_KEY_THAI_SOSALA XK_Thai_sosala
    #define PB_KEY_THAI_SORUSI XK_Thai_sorusi
    #define PB_KEY_THAI_SOSUA XK_Thai_sosua
    #define PB_KEY_THAI_HOHIP XK_Thai_hohip
    #define PB_KEY_THAI_LOCHULA XK_Thai_lochula
    #define PB_KEY_THAI_OANG XK_Thai_oang
    #define PB_KEY_THAI_HONOKHUK XK_Thai_honokhuk
    #define PB_KEY_THAI_PAIYANNOI XK_Thai_paiyannoi
    #define PB_KEY_THAI_SARAA XK_Thai_saraa
    #define PB_KEY_THAI_MAIHANAKAT XK_Thai_maihanakat
    #define PB_KEY_THAI_SARAAA XK_Thai_saraaa
    #define PB_KEY_THAI_SARAAM XK_Thai_saraam
    #define PB_KEY_THAI_SARAI XK_Thai_sarai
    #define PB_KEY_THAI_SARAII XK_Thai_saraii
    #define PB_KEY_THAI_SARAUE XK_Thai_saraue
    #define PB_KEY_THAI_SARAUEE XK_Thai_sarauee
    #define PB_KEY_THAI_SARAU XK_Thai_sarau
    #define PB_KEY_THAI_SARAUU XK_Thai_sarauu
    #define PB_KEY_THAI_PHINTHU XK_Thai_phinthu
    #define PB_KEY_THAI_MAIHANAKAT_MAITHO XK_Thai_maihanakat_maitho
    #define PB_KEY_THAI_BAHT XK_Thai_baht
    #define PB_KEY_THAI_SARAE XK_Thai_sarae
    #define PB_KEY_THAI_SARAAE XK_Thai_saraae
    #define PB_KEY_THAI_SARAO XK_Thai_sarao
    #define PB_KEY_THAI_SARAAIMAIMUAN XK_Thai_saraaimaimuan
    #define PB_KEY_THAI_SARAAIMAIMALAI XK_Thai_saraaimaimalai
    #define PB_KEY_THAI_LAKKHANGYAO XK_Thai_lakkhangyao
    #define PB_KEY_THAI_MAIYAMOK XK_Thai_maiyamok
    #define PB_KEY_THAI_MAITAIKHU XK_Thai_maitaikhu
    #define PB_KEY_THAI_MAIEK XK_Thai_maiek
    #define PB_KEY_THAI_MAITHO XK_Thai_maitho
    #define PB_KEY_THAI_MAITRI XK_Thai_maitri
    #define PB_KEY_THAI_MAICHATTAWA XK_Thai_maichattawa
    #define PB_KEY_THAI_THANTHAKHAT XK_Thai_thanthakhat
    #define PB_KEY_THAI_NIKHAHIT XK_Thai_nikhahit
    #define PB_KEY_THAI_LEKSUN XK_Thai_leksun
    #define PB_KEY_THAI_LEKNUNG XK_Thai_leknung
    #define PB_KEY_THAI_LEKSONG XK_Thai_leksong
    #define PB_KEY_THAI_LEKSAM XK_Thai_leksam
    #define PB_KEY_THAI_LEKSI XK_Thai_leksi
    #define PB_KEY_THAI_LEKHA XK_Thai_lekha
    #define PB_KEY_THAI_LEKHOK XK_Thai_lekhok
    #define PB_KEY_THAI_LEKCHET XK_Thai_lekchet
    #define PB_KEY_THAI_LEKPAET XK_Thai_lekpaet
    #define PB_KEY_THAI_LEKKAO XK_Thai_lekkao
    #endif


    /*KOREANKorean
    *BYTEByte
    */

    #ifdef XK_KOREAN
    #define PB_KEY_HANGUL XK_Hangul
    #define PB_KEY_HANGUL_START XK_Hangul_Start
    #define PB_KEY_HANGUL_END XK_Hangul_End
    #define PB_KEY_HANGUL_HANJA XK_Hangul_Hanja
    #define PB_KEY_HANGUL_JAMO XK_Hangul_Jamo
    #define PB_KEY_HANGUL_ROMAJA XK_Hangul_Romaja
    #define PB_KEY_HANGUL_CODEINPUT XK_Hangul_Codeinput
    #define PB_KEY_HANGUL_JEONJA XK_Hangul_Jeonja
    #define PB_KEY_HANGUL_BANJA XK_Hangul_Banja
    #define PB_KEY_HANGUL_PREHANJA XK_Hangul_PreHanja
    #define PB_KEY_HANGUL_POSTHANJA XK_Hangul_PostHanja
    #define PB_KEY_HANGUL_SINGLECANDIDATE XK_Hangul_SingleCandidate
    #define PB_KEY_HANGUL_MULTIPLECANDIDATE XK_Hangul_MultipleCandidate
    #define PB_KEY_HANGUL_PREVIOUSCANDIDATE XK_Hangul_PreviousCandidate
    #define PB_KEY_HANGUL_SPECIAL XK_Hangul_Special
    #define PB_KEY_HANGUL_SWITCH XK_Hangul_switch

    // HangulCONSONANTConsonant
    #define PB_KEY_HANGUL_KIYEOG XK_Hangul_Kiyeog
    #define PB_KEY_HANGUL_SSANGKIYEOG XK_Hangul_SsangKiyeog
    #define PB_KEY_HANGUL_KIYEOGSIOS XK_Hangul_KiyeogSios
    #define PB_KEY_HANGUL_NIEUN XK_Hangul_Nieun
    #define PB_KEY_HANGUL_NIEUNJIEUJ XK_Hangul_NieunJieuj
    #define PB_KEY_HANGUL_NIEUNHIEUH XK_Hangul_NieunHieuh
    #define PB_KEY_HANGUL_DIKEUD XK_Hangul_Dikeud
    #define PB_KEY_HANGUL_SSANGDIKEUD XK_Hangul_SsangDikeud
    #define PB_KEY_HANGUL_RIEUL XK_Hangul_Rieul
    #define PB_KEY_HANGUL_RIEULKIYEOG XK_Hangul_RieulKiyeog
    #define PB_KEY_HANGUL_RIEULMIEUM XK_Hangul_RieulMieum
    #define PB_KEY_HANGUL_RIEULPIEUB XK_Hangul_RieulPieub
    #define PB_KEY_HANGUL_RIEULSIOS XK_Hangul_RieulSios
    #define PB_KEY_HANGUL_RIEULTIEUT XK_Hangul_RieulTieut
    #define PB_KEY_HANGUL_RIEULPHIEUF XK_Hangul_RieulPhieuf
    #define PB_KEY_HANGUL_RIEULHIEUH XK_Hangul_RieulHieuh
    #define PB_KEY_HANGUL_MIEUM XK_Hangul_Mieum
    #define PB_KEY_HANGUL_PIEUB XK_Hangul_Pieub
    #define PB_KEY_HANGUL_SSANGPIEUB XK_Hangul_SsangPieub
    #define PB_KEY_HANGUL_PIEUBSIOS XK_Hangul_PieubSios
    #define PB_KEY_HANGUL_SIOS XK_Hangul_Sios
    #define PB_KEY_HANGUL_SSANGSIOS XK_Hangul_SsangSios
    #define PB_KEY_HANGUL_IEUNG XK_Hangul_Ieung
    #define PB_KEY_HANGUL_JIEUJ XK_Hangul_Jieuj
    #define PB_KEY_HANGUL_SSANGJIEUJ XK_Hangul_SsangJieuj
    #define PB_KEY_HANGUL_CIEUC XK_Hangul_Cieuc
    #define PB_KEY_HANGUL_KHIEUQ XK_Hangul_Khieuq
    #define PB_KEY_HANGUL_TIEUT XK_Hangul_Tieut
    #define PB_KEY_HANGUL_PHIEUF XK_Hangul_Phieuf
    #define PB_KEY_HANGUL_HIEUH XK_Hangul_Hieuh

    //Hangul Vowel
    #define PB_KEY_HANGUL_A XK_Hangul_A
    #define PB_KEY_HANGUL_AE XK_Hangul_AE
    #define PB_KEY_HANGUL_YA XK_Hangul_YA
    #define PB_KEY_HANGUL_YAE XK_Hangul_YAE
    #define PB_KEY_HANGUL_EO XK_Hangul_EO
    #define PB_KEY_HANGUL_E XK_Hangul_E
    #define PB_KEY_HANGUL_YEO XK_Hangul_YEO
    #define PB_KEY_HANGUL_YE XK_Hangul_YE
    #define PB_KEY_HANGUL_O XK_Hangul_O
    #define PB_KEY_HANGUL_WA XK_Hangul_WA
    #define PB_KEY_HANGUL_WAE XK_Hangul_WAE
    #define PB_KEY_HANGUL_OE XK_Hangul_OE
    #define PB_KEY_HANGUL_YO XK_Hangul_YO
    #define PB_KEY_HANGUL_U XK_Hangul_U
    #define PB_KEY_HANGUL_WEO XK_Hangul_WEO
    #define PB_KEY_HANGUL_WE XK_Hangul_WE
    #define PB_KEY_HANGUL_WI XK_Hangul_WI
    #define PB_KEY_HANGUL_YU XK_Hangul_YU
    #define PB_KEY_HANGUL_EU XK_Hangul_EU
    #define PB_KEY_HANGUL_YI XK_Hangul_YI
    #define PB_KEY_HANGUL_I XK_Hangul_I

    //Hangul Syllable-final
    #define PB_KEY_HANGUL_J_KIYEOG XK_Hangul_J_Kiyeog
    #define PB_KEY_HANGUL_J_SSANGKIYEOG XK_Hangul_J_SsangKiyeog
    #define PB_KEY_HANGUL_J_KIYEOGSIOS XK_Hangul_J_KiyeogSios
    #define PB_KEY_HANGUL_J_NIEUN XK_Hangul_J_Nieun
    #define PB_KEY_HANGUL_J_NIEUNJIEUJ XK_Hangul_J_NieunJieuj
    #define PB_KEY_HANGUL_J_NIEUNHIEUH XK_Hangul_J_NieunHieuh
    #define PB_KEY_HANGUL_J_DIKEUD XK_Hangul_J_Dikeud
    #define PB_KEY_HANGUL_J_RIEUL XK_Hangul_J_Rieul
    #define PB_KEY_HANGUL_J_RIEULKIYEOG XK_Hangul_J_RieulKiyeog
    #define PB_KEY_HANGUL_J_RIEULMIEUM XK_Hangul_J_RieulMieum
    #define PB_KEY_HANGUL_J_RIEULPIEUB XK_Hangul_J_RieulPieub
    #define PB_KEY_HANGUL_J_RIEULSIOS XK_Hangul_J_RieulSios
    #define PB_KEY_HANGUL_J_RIEULTIEUT XK_Hangul_J_RieulTieut
    #define PB_KEY_HANGUL_J_RIEULPHIEUF XK_Hangul_J_RieulPhieuf
    #define PB_KEY_HANGUL_J_RIEULHIEUH XK_Hangul_J_RieulHieuh
    #define PB_KEY_HANGUL_J_MIEUM XK_Hangul_J_Mieum
    #define PB_KEY_HANGUL_J_PIEUB XK_Hangul_J_Pieub
    #define PB_KEY_HANGUL_J_PIEUBSIOS XK_Hangul_J_PieubSios
    #define PB_KEY_HANGUL_J_SIOS XK_Hangul_J_Sios
    #define PB_KEY_HANGUL_J_SSANGSIOS XK_Hangul_J_SsangSios
    #define PB_KEY_HANGUL_J_IEUNG XK_Hangul_J_Ieung
    #define PB_KEY_HANGUL_J_JIEUJ XK_Hangul_J_Jieuj
    #define PB_KEY_HANGUL_J_CIEUC XK_Hangul_J_Cieuc
    #define PB_KEY_HANGUL_J_KHIEUQ XK_Hangul_J_Khieuq
    #define PB_KEY_HANGUL_J_TIEUT XK_Hangul_J_Tieut
    #define PB_KEY_HANGUL_J_PHIEUF XK_Hangul_J_Phieuf
    #define PB_KEY_HANGUL_J_HIEUH XK_Hangul_J_Hieuh

    //Ancient Hangul
    #define PB_KEY_HANGUL_RIEULYEORINHIEUH XK_Hangul_RieulYeorinHieuh
    #define PB_KEY_HANGUL_SUNKYEONGEUMMIEUM XK_Hangul_SunkyeongeumMieum
    #define PB_KEY_HANGUL_SUNKYEONGEUMPIEUB XK_Hangul_SunkyeongeumPieub
    #define PB_KEY_HANGUL_PANSIOS XK_Hangul_PanSios
    #define PB_KEY_HANGUL_KKOGJIDALRINIEUNG XK_Hangul_KkogjiDalrinIeung
    #define PB_KEY_HANGUL_SUNKYEONGEUMPHIEUF XK_Hangul_SunkyeongeumPhieuf
    #define PB_KEY_HANGUL_YEORINHIEUH XK_Hangul_YeorinHieuh

    //Ancient Hangul
    #define PB_KEY_HANGUL_ARAEA XK_Hangul_AraeA
    #define PB_KEY_HANGUL_ARAEAE XK_Hangul_AraeAE

    //Ancient Hangul
    #define PB_KEY_HANGUL_J_PANSIOS XK_Hangul_J_PanSios
    #define PB_KEY_HANGUL_J_KKOGJIDALRINIEUNG XK_Hangul_J_KkogjiDalrinIeung
    #define PB_KEY_HANGUL_J_YEORINHIEUH XK_Hangul_J_YeorinHieuh

    //Korean currency
    #define PB_KEY_KOREAN_WON XK_Korean_Won

    #endif

    /*
    *ARMENIANArmenian
    */

    #ifdef XK_ARMENIAN
    #define PB_KEY_ARMENIAN_LIGATURE_EW XK_Armenian_ligature_ew
    #define PB_KEY_ARMENIAN_FULL_STOP XK_Armenian_full_stop
    #define PB_KEY_ARMENIAN_VERJAKET XK_Armenian_verjaket
    #define PB_KEY_ARMENIAN_SEPARATION_MARK XK_Armenian_separation_mark
    #define PB_KEY_ARMENIAN_BUT XK_Armenian_but
    #define PB_KEY_ARMENIAN_HYPHEN XK_Armenian_hyphen
    #define PB_KEY_ARMENIAN_YENTAMNA XK_Armenian_yentamna
    #define PB_KEY_ARMENIAN_EXCLAM XK_Armenian_exclam
    #define PB_KEY_ARMENIAN_AMANAK XK_Armenian_amanak
    #define PB_KEY_ARMENIAN_ACCENT XK_Armenian_accent
    #define PB_KEY_ARMENIAN_SHESHT XK_Armenian_shesht
    #define PB_KEY_ARMENIAN_QUESTION XK_Armenian_question
    #define PB_KEY_ARMENIAN_PARUYK XK_Armenian_paruyk
    #define PB_KEY_CAPITAL_ARMENIAN_AYB XK_Armenian_AYB
    #define PB_KEY_ARMENIAN_AYB XK_Armenian_ayb
    #define PB_KEY_CAPITAL_ARMENIAN_BEN XK_Armenian_BEN
    #define PB_KEY_ARMENIAN_BEN XK_Armenian_ben
    #define PB_KEY_CAPITAL_ARMENIAN_GIM XK_Armenian_GIM
    #define PB_KEY_ARMENIAN_GIM XK_Armenian_gim
    #define PB_KEY_CAPITAL_ARMENIAN_DA XK_Armenian_DA
    #define PB_KEY_ARMENIAN_DA XK_Armenian_da
    #define PB_KEY_CAPITAL_ARMENIAN_YECH XK_Armenian_YECH
    #define PB_KEY_ARMENIAN_YECH XK_Armenian_yech
    #define PB_KEY_CAPITAL_ARMENIAN_ZA XK_Armenian_ZA
    #define PB_KEY_ARMENIAN_ZA XK_Armenian_za
    #define PB_KEY_CAPITAL_ARMENIAN_E XK_Armenian_E
    #define PB_KEY_ARMENIAN_E XK_Armenian_e
    #define PB_KEY_CAPITAL_ARMENIAN_AT XK_Armenian_AT
    #define PB_KEY_ARMENIAN_AT XK_Armenian_at
    #define PB_KEY_CAPITAL_ARMENIAN_TO XK_Armenian_TO
    #define PB_KEY_ARMENIAN_TO XK_Armenian_to
    #define PB_KEY_CAPITAL_ARMENIAN_ZHE XK_Armenian_ZHE
    #define PB_KEY_ARMENIAN_ZHE XK_Armenian_zhe
    #define PB_KEY_CAPITAL_ARMENIAN_INI XK_Armenian_INI
    #define PB_KEY_ARMENIAN_INI XK_Armenian_ini
    #define PB_KEY_CAPITAL_ARMENIAN_LYUN XK_Armenian_LYUN
    #define PB_KEY_ARMENIAN_LYUN XK_Armenian_lyun
    #define PB_KEY_CAPITAL_ARMENIAN_KHE XK_Armenian_KHE
    #define PB_KEY_ARMENIAN_KHE XK_Armenian_khe
    #define PB_KEY_CAPITAL_ARMENIAN_TSA XK_Armenian_TSA
    #define PB_KEY_ARMENIAN_TSA XK_Armenian_tsa
    #define PB_KEY_CAPITAL_ARMENIAN_KEN XK_Armenian_KEN
    #define PB_KEY_ARMENIAN_KEN XK_Armenian_ken
    #define PB_KEY_CAPITAL_ARMENIAN_HO XK_Armenian_HO
    #define PB_KEY_ARMENIAN_HO XK_Armenian_ho
    #define PB_KEY_CAPITAL_ARMENIAN_DZA XK_Armenian_DZA
    #define PB_KEY_ARMENIAN_DZA XK_Armenian_dza
    #define PB_KEY_CAPITAL_ARMENIAN_GHAT XK_Armenian_GHAT
    #define PB_KEY_ARMENIAN_GHAT XK_Armenian_ghat
    #define PB_KEY_CAPITAL_ARMENIAN_TCHE XK_Armenian_TCHE
    #define PB_KEY_ARMENIAN_TCHE XK_Armenian_tche
    #define PB_KEY_CAPITAL_ARMENIAN_MEN XK_Armenian_MEN
    #define PB_KEY_ARMENIAN_MEN XK_Armenian_men
    #define PB_KEY_CAPITAL_ARMENIAN_HI XK_Armenian_HI
    #define PB_KEY_ARMENIAN_HI XK_Armenian_hi
    #define PB_KEY_CAPITAL_ARMENIAN_NU XK_Armenian_NU
    #define PB_KEY_ARMENIAN_NU XK_Armenian_nu
    #define PB_KEY_CAPITAL_ARMENIAN_SHA XK_Armenian_SHA
    #define PB_KEY_ARMENIAN_SHA XK_Armenian_sha
    #define PB_KEY_CAPITAL_ARMENIAN_VO XK_Armenian_VO
    #define PB_KEY_ARMENIAN_VO XK_Armenian_vo
    #define PB_KEY_CAPITAL_ARMENIAN_CHA XK_Armenian_CHA
    #define PB_KEY_ARMENIAN_CHA XK_Armenian_cha
    #define PB_KEY_CAPITAL_ARMENIAN_PE XK_Armenian_PE
    #define PB_KEY_ARMENIAN_PE XK_Armenian_pe
    #define PB_KEY_CAPITAL_ARMENIAN_JE XK_Armenian_JE
    #define PB_KEY_ARMENIAN_JE XK_Armenian_je
    #define PB_KEY_CAPITAL_ARMENIAN_RA XK_Armenian_RA
    #define PB_KEY_ARMENIAN_RA XK_Armenian_ra
    #define PB_KEY_CAPITAL_ARMENIAN_SE XK_Armenian_SE
    #define PB_KEY_ARMENIAN_SE XK_Armenian_se
    #define PB_KEY_CAPITAL_ARMENIAN_VEV XK_Armenian_VEV
    #define PB_KEY_ARMENIAN_VEV XK_Armenian_vev
    #define PB_KEY_CAPITAL_ARMENIAN_TYUN XK_Armenian_TYUN
    #define PB_KEY_ARMENIAN_TYUN XK_Armenian_tyun
    #define PB_KEY_CAPITAL_ARMENIAN_RE XK_Armenian_RE
    #define PB_KEY_ARMENIAN_RE XK_Armenian_re
    #define PB_KEY_CAPITAL_ARMENIAN_TSO XK_Armenian_TSO
    #define PB_KEY_ARMENIAN_TSO XK_Armenian_tso
    #define PB_KEY_CAPITAL_ARMENIAN_VYUN XK_Armenian_VYUN
    #define PB_KEY_ARMENIAN_VYUN XK_Armenian_vyun
    #define PB_KEY_CAPITAL_ARMENIAN_PYUR XK_Armenian_PYUR
    #define PB_KEY_ARMENIAN_PYUR XK_Armenian_pyur
    #define PB_KEY_CAPITAL_ARMENIAN_KE XK_Armenian_KE
    #define PB_KEY_ARMENIAN_KE XK_Armenian_ke
    #define PB_KEY_CAPITAL_ARMENIAN_O XK_Armenian_O
    #define PB_KEY_ARMENIAN_O XK_Armenian_o
    #define PB_KEY_CAPITAL_ARMENIAN_FE XK_Armenian_FE
    #define PB_KEY_ARMENIAN_FE XK_Armenian_fe
    #define PB_KEY_ARMENIAN_APOSTROPHE XK_Armenian_apostrophe
    #endif

    /*
    *GEORGIANGeorgian
    */

    #ifdef XK_GEORGIAN
    #define PB_KEY_GEORGIAN_AN XK_Georgian_an
    #define PB_KEY_GEORGIAN_BAN XK_Georgian_ban
    #define PB_KEY_GEORGIAN_GAN XK_Georgian_gan
    #define PB_KEY_GEORGIAN_DON XK_Georgian_don
    #define PB_KEY_GEORGIAN_EN XK_Georgian_en
    #define PB_KEY_GEORGIAN_VIN XK_Georgian_vin
    #define PB_KEY_GEORGIAN_ZEN XK_Georgian_zen
    #define PB_KEY_GEORGIAN_TAN XK_Georgian_tan
    #define PB_KEY_GEORGIAN_IN XK_Georgian_in
    #define PB_KEY_GEORGIAN_KAN XK_Georgian_kan
    #define PB_KEY_GEORGIAN_LAS XK_Georgian_las
    #define PB_KEY_GEORGIAN_MAN XK_Georgian_man
    #define PB_KEY_GEORGIAN_NAR XK_Georgian_nar
    #define PB_KEY_GEORGIAN_ON XK_Georgian_on
    #define PB_KEY_GEORGIAN_PAR XK_Georgian_par
    #define PB_KEY_GEORGIAN_ZHAR XK_Georgian_zhar
    #define PB_KEY_GEORGIAN_RAE XK_Georgian_rae
    #define PB_KEY_GEORGIAN_SAN XK_Georgian_san
    #define PB_KEY_GEORGIAN_TAR XK_Georgian_tar
    #define PB_KEY_GEORGIAN_UN XK_Georgian_un
    #define PB_KEY_GEORGIAN_PHAR XK_Georgian_phar
    #define PB_KEY_GEORGIAN_KHAR XK_Georgian_khar
    #define PB_KEY_GEORGIAN_GHAN XK_Georgian_ghan
    #define PB_KEY_GEORGIAN_QAR XK_Georgian_qar
    #define PB_KEY_GEORGIAN_SHIN XK_Georgian_shin
    #define PB_KEY_GEORGIAN_CHIN XK_Georgian_chin
    #define PB_KEY_GEORGIAN_CAN XK_Georgian_can
    #define PB_KEY_GEORGIAN_JIL XK_Georgian_jil
    #define PB_KEY_GEORGIAN_CIL XK_Georgian_cil
    #define PB_KEY_GEORGIAN_CHAR XK_Georgian_char
    #define PB_KEY_GEORGIAN_XAN XK_Georgian_xan
    #define PB_KEY_GEORGIAN_JHAN XK_Georgian_jhan
    #define PB_KEY_GEORGIAN_HAE XK_Georgian_hae
    #define PB_KEY_GEORGIAN_HE XK_Georgian_he
    #define PB_KEY_GEORGIAN_HIE XK_Georgian_hie
    #define PB_KEY_GEORGIAN_WE XK_Georgian_we
    #define PB_KEY_GEORGIAN_HAR XK_Georgian_har
    #define PB_KEY_GEORGIAN_HOE XK_Georgian_hoe
    #define PB_KEY_GEORGIAN_FI XK_Georgian_fi
    #endif

    /** Azeri (and other Turkic or Caucasian languages) */        



    #ifdef XK_CAUCASUS
    /*latin*/
    #define PB_KEY_CAPITAL_XABOVEDOT XK_Xabovedot
    #define PB_KEY_CAPITAL_IBREVE XK_Ibreve
    #define PB_KEY_CAPITAL_ZSTROKE XK_Zstroke
    #define PB_KEY_CAPITAL_GCARON XK_Gcaron
    #define PB_KEY_CAPITAL_OCARON XK_Ocaron
    #define PB_KEY_CAPITAL_OBARRED XK_Obarred
    #define PB_KEY_XABOVEDOT XK_xabovedot
    #define PB_KEY_IBREVE XK_ibreve
    #define PB_KEY_ZSTROKE XK_zstroke
    #define PB_KEY_GCARON XK_gcaron
    #define PB_KEY_OCARON XK_ocaron
    #define PB_KEY_OBARRED XK_obarred
    #define PB_KEY_CAPITAL_SCHWA XK_SCHWA
    #define PB_KEY_SCHWA XK_schwa
    #define PB_KEY_CAPITAL_EZH XK_EZH
    #define PB_KEY_EZH XK_ezh
    //Inupiak
    #define PB_KEY_CAPITAL_LBELOWDOT XK_Lbelowdot
    #define PB_KEY_LBELOWDOT XK_lbelowdot
    #endif

    /*
    *VIETNAMESEVietnamese
    */

    #ifdef XK_VIETNAMESE
    #define PB_KEY_CAPITAL_ABELOWDOT XK_Abelowdot
    #define PB_KEY_ABELOWDOT XK_abelowdot
    #define PB_KEY_CAPITAL_AHOOK XK_Ahook
    #define PB_KEY_AHOOK XK_ahook
    #define PB_KEY_CAPITAL_ACIRCUMFLEXACUTE XK_Acircumflexacute
    #define PB_KEY_ACIRCUMFLEXACUTE XK_acircumflexacute
    #define PB_KEY_CAPITAL_ACIRCUMFLEXGRAVE XK_Acircumflexgrave
    #define PB_KEY_ACIRCUMFLEXGRAVE XK_acircumflexgrave
    #define PB_KEY_CAPITAL_ACIRCUMFLEXHOOK XK_Acircumflexhook
    #define PB_KEY_ACIRCUMFLEXHOOK XK_acircumflexhook
    #define PB_KEY_CAPITAL_ACIRCUMFLEXTILDE XK_Acircumflextilde
    #define PB_KEY_ACIRCUMFLEXTILDE XK_acircumflextilde
    #define PB_KEY_CAPITAL_ACIRCUMFLEXBELOWDOT XK_Acircumflexbelowdot
    #define PB_KEY_ACIRCUMFLEXBELOWDOT XK_acircumflexbelowdot
    #define PB_KEY_CAPITAL_ABREVEACUTE XK_Abreveacute
    #define PB_KEY_ABREVEACUTE XK_abreveacute
    #define PB_KEY_CAPITAL_ABREVEGRAVE XK_Abrevegrave
    #define PB_KEY_ABREVEGRAVE XK_abrevegrave
    #define PB_KEY_CAPITAL_ABREVEHOOK XK_Abrevehook
    #define PB_KEY_ABREVEHOOK XK_abrevehook
    #define PB_KEY_CAPITAL_ABREVETILDE XK_Abrevetilde
    #define PB_KEY_ABREVETILDE XK_abrevetilde
    #define PB_KEY_CAPITAL_ABREVEBELOWDOT XK_Abrevebelowdot
    #define PB_KEY_ABREVEBELOWDOT XK_abrevebelowdot
    #define PB_KEY_CAPITAL_EBELOWDOT XK_Ebelowdot
    #define PB_KEY_EBELOWDOT XK_ebelowdot
    #define PB_KEY_CAPITAL_EHOOK XK_Ehook
    #define PB_KEY_EHOOK XK_ehook
    #define PB_KEY_CAPITAL_ETILDE XK_Etilde
    #define PB_KEY_ETILDE XK_etilde
    #define PB_KEY_CAPITAL_ECIRCUMFLEXACUTE XK_Ecircumflexacute
    #define PB_KEY_ECIRCUMFLEXACUTE XK_ecircumflexacute
    #define PB_KEY_CAPITAL_ECIRCUMFLEXGRAVE XK_Ecircumflexgrave
    #define PB_KEY_ECIRCUMFLEXGRAVE XK_ecircumflexgrave
    #define PB_KEY_CAPITAL_ECIRCUMFLEXHOOK XK_Ecircumflexhook
    #define PB_KEY_ECIRCUMFLEXHOOK XK_ecircumflexhook
    #define PB_KEY_CAPITAL_ECIRCUMFLEXTILDE XK_Ecircumflextilde
    #define PB_KEY_ECIRCUMFLEXTILDE XK_ecircumflextilde
    #define PB_KEY_CAPITAL_ECIRCUMFLEXBELOWDOT XK_Ecircumflexbelowdot
    #define PB_KEY_ECIRCUMFLEXBELOWDOT XK_ecircumflexbelowdot
    #define PB_KEY_CAPITAL_IHOOK XK_Ihook
    #define PB_KEY_IHOOK XK_ihook
    #define PB_KEY_CAPITAL_IBELOWDOT XK_Ibelowdot
    #define PB_KEY_IBELOWDOT XK_ibelowdot
    #define PB_KEY_CAPITAL_OBELOWDOT XK_Obelowdot
    #define PB_KEY_OBELOWDOT XK_obelowdot
    #define PB_KEY_CAPITAL_OHOOK XK_Ohook
    #define PB_KEY_OHOOK XK_ohook
    #define PB_KEY_CAPITAL_OCIRCUMFLEXACUTE XK_Ocircumflexacute
    #define PB_KEY_OCIRCUMFLEXACUTE XK_ocircumflexacute
    #define PB_KEY_CAPITAL_OCIRCUMFLEXGRAVE XK_Ocircumflexgrave
    #define PB_KEY_OCIRCUMFLEXGRAVE XK_ocircumflexgrave
    #define PB_KEY_CAPITAL_OCIRCUMFLEXHOOK XK_Ocircumflexhook
    #define PB_KEY_OCIRCUMFLEXHOOK XK_ocircumflexhook
    #define PB_KEY_CAPITAL_OCIRCUMFLEXTILDE XK_Ocircumflextilde
    #define PB_KEY_OCIRCUMFLEXTILDE XK_ocircumflextilde
    #define PB_KEY_CAPITAL_OCIRCUMFLEXBELOWDOT XK_Ocircumflexbelowdot
    #define PB_KEY_OCIRCUMFLEXBELOWDOT XK_ocircumflexbelowdot
    #define PB_KEY_CAPITAL_OHORNACUTE XK_Ohornacute
    #define PB_KEY_OHORNACUTE XK_ohornacute
    #define PB_KEY_CAPITAL_OHORNGRAVE XK_Ohorngrave
    #define PB_KEY_OHORNGRAVE XK_ohorngrave
    #define PB_KEY_CAPITAL_OHORNHOOK XK_Ohornhook
    #define PB_KEY_OHORNHOOK XK_ohornhook
    #define PB_KEY_CAPITAL_OHORNTILDE XK_Ohorntilde
    #define PB_KEY_OHORNTILDE XK_ohorntilde
    #define PB_KEY_CAPITAL_OHORNBELOWDOT XK_Ohornbelowdot
    #define PB_KEY_OHORNBELOWDOT XK_ohornbelowdot
    #define PB_KEY_CAPITAL_UBELOWDOT XK_Ubelowdot
    #define PB_KEY_UBELOWDOT XK_ubelowdot
    #define PB_KEY_CAPITAL_UHOOK XK_Uhook
    #define PB_KEY_UHOOK XK_uhook
    #define PB_KEY_CAPITAL_UHORNACUTE XK_Uhornacute
    #define PB_KEY_UHORNACUTE XK_uhornacute
    #define PB_KEY_CAPITAL_UHORNGRAVE XK_Uhorngrave
    #define PB_KEY_UHORNGRAVE XK_uhorngrave
    #define PB_KEY_CAPITAL_UHORNHOOK XK_Uhornhook
    #define PB_KEY_UHORNHOOK XK_uhornhook
    #define PB_KEY_CAPITAL_UHORNTILDE XK_Uhorntilde
    #define PB_KEY_UHORNTILDE XK_uhorntilde
    #define PB_KEY_CAPITAL_UHORNBELOWDOT XK_Uhornbelowdot
    #define PB_KEY_UHORNBELOWDOT XK_uhornbelowdot
    #define PB_KEY_CAPITAL_YBELOWDOT XK_Ybelowdot
    #define PB_KEY_YBELOWDOT XK_ybelowdot
    #define PB_KEY_CAPITAL_YHOOK XK_Yhook
    #define PB_KEY_YHOOK XK_yhook
    #define PB_KEY_CAPITAL_YTILDE XK_Ytilde
    #define PB_KEY_YTILDE XK_ytilde
    #define PB_KEY_CAPITAL_OHORN XK_Ohorn
    #define PB_KEY_OHORN XK_ohorn
    #define PB_KEY_CAPITAL_UHORN XK_Uhorn
    #define PB_KEY_UHORN XK_uhorn
    #define PB_KEY_COMBINING_TILDE XK_combining_tilde
    #define PB_KEY_COMBINING_GRAVE XK_combining_grave
    #define PB_KEY_COMBINING_ACUTE XK_combining_acute
    #define PB_KEY_COMBINING_HOOK XK_combining_hook
    #define PB_KEY_COMBINING_BELOWDOT XK_combining_belowdot

    #endif

    #ifdef XK_CURRENCY
    #define PB_KEY_ECUSIGN XK_EcuSign
    #define PB_KEY_COLONSIGN XK_ColonSign
    #define PB_KEY_CRUZEIROSIGN XK_CruzeiroSign
    #define PB_KEY_FFRANCSIGN XK_FFrancSign
    #define PB_KEY_LIRASIGN XK_LiraSign
    #define PB_KEY_MILLSIGN XK_MillSign
    #define PB_KEY_NAIRASIGN XK_NairaSign
    #define PB_KEY_PESETASIGN XK_PesetaSign
    #define PB_KEY_RUPEESIGN XK_RupeeSign
    #define PB_KEY_WONSIGN XK_WonSign
    #define PB_KEY_NEWSHEQELSIGN XK_NewSheqelSign
    #define PB_KEY_DONGSIGN XK_DongSign
    #define PB_KEY_EUROSIGN XK_EuroSign
    #endif

    #ifdef XK_MATHEMATICAL
    /*one, two and three are defined above. */ 
    #define PB_KEY_ZEROSUPERIOR XK_zerosuperior
    #define PB_KEY_FOURSUPERIOR XK_foursuperior
    #define PB_KEY_FIVESUPERIOR XK_fivesuperior
    #define PB_KEY_SIXSUPERIOR XK_sixsuperior
    #define PB_KEY_SEVENSUPERIOR XK_sevensuperior
    #define PB_KEY_EIGHTSUPERIOR XK_eightsuperior
    #define PB_KEY_NINESUPERIOR XK_ninesuperior
    #define PB_KEY_ZEROSUBSCRIPT XK_zerosubscript
    #define PB_KEY_ONESUBSCRIPT XK_onesubscript
    #define PB_KEY_TWOSUBSCRIPT XK_twosubscript
    #define PB_KEY_THREESUBSCRIPT XK_threesubscript
    #define PB_KEY_FOURSUBSCRIPT XK_foursubscript
    #define PB_KEY_FIVESUBSCRIPT XK_fivesubscript
    #define PB_KEY_SIXSUBSCRIPT XK_sixsubscript
    #define PB_KEY_SEVENSUBSCRIPT XK_sevensubscript
    #define PB_KEY_EIGHTSUBSCRIPT XK_eightsubscript
    #define PB_KEY_NINESUBSCRIPT XK_ninesubscript
    #define PB_KEY_PARTDIFFERENTIAL XK_partdifferential
    #define PB_KEY_EMPTYSET XK_emptyset
    #define PB_KEY_ELEMENTOF XK_elementof
    #define PB_KEY_NOTELEMENTOF XK_notelementof
    #define PB_KEY_CONTAINSAS XK_containsas
    #define PB_KEY_SQUAREROOT XK_squareroot
    #define PB_KEY_CUBEROOT XK_cuberoot
    #define PB_KEY_FOURTHROOT XK_fourthroot
    #define PB_KEY_DINTEGRAL XK_dintegral
    #define PB_KEY_TINTEGRAL XK_tintegral
    #define PB_KEY_BECAUSE XK_because
    #define PB_KEY_APPROXEQ XK_approxeq
    #define PB_KEY_NOTAPPROXEQ XK_notapproxeq
    #define PB_KEY_NOTIDENTICAL XK_notidentical
    #define PB_KEY_STRICTEQ XK_stricteq
    #endif

    #ifdef XK_BRAILLE
    #define PB_KEY_BRAILLE_DOT_1 XK_braille_dot_1
    #define PB_KEY_BRAILLE_DOT_2 XK_braille_dot_2
    #define PB_KEY_BRAILLE_DOT_3 XK_braille_dot_3
    #define PB_KEY_BRAILLE_DOT_4 XK_braille_dot_4
    #define PB_KEY_BRAILLE_DOT_5 XK_braille_dot_5
    #define PB_KEY_BRAILLE_DOT_6 XK_braille_dot_6
    #define PB_KEY_BRAILLE_DOT_7 XK_braille_dot_7
    #define PB_KEY_BRAILLE_DOT_8 XK_braille_dot_8
    #define PB_KEY_BRAILLE_DOT_9 XK_braille_dot_9
    #define PB_KEY_BRAILLE_DOT_10 XK_braille_dot_10
    #define PB_KEY_BRAILLE_BLANK XK_braille_blank
    #define PB_KEY_BRAILLE_DOTS_1 XK_braille_dots_1
    #define PB_KEY_BRAILLE_DOTS_2 XK_braille_dots_2
    #define PB_KEY_BRAILLE_DOTS_12 XK_braille_dots_12
    #define PB_KEY_BRAILLE_DOTS_3 XK_braille_dots_3
    #define PB_KEY_BRAILLE_DOTS_13 XK_braille_dots_13
    #define PB_KEY_BRAILLE_DOTS_23 XK_braille_dots_23
    #define PB_KEY_BRAILLE_DOTS_123 XK_braille_dots_123
    #define PB_KEY_BRAILLE_DOTS_4 XK_braille_dots_4
    #define PB_KEY_BRAILLE_DOTS_14 XK_braille_dots_14
    #define PB_KEY_BRAILLE_DOTS_24 XK_braille_dots_24
    #define PB_KEY_BRAILLE_DOTS_124 XK_braille_dots_124
    #define PB_KEY_BRAILLE_DOTS_34 XK_braille_dots_34
    #define PB_KEY_BRAILLE_DOTS_134 XK_braille_dots_134
    #define PB_KEY_BRAILLE_DOTS_234 XK_braille_dots_234
    #define PB_KEY_BRAILLE_DOTS_1234 XK_braille_dots_1234
    #define PB_KEY_BRAILLE_DOTS_5 XK_braille_dots_5
    #define PB_KEY_BRAILLE_DOTS_15 XK_braille_dots_15
    #define PB_KEY_BRAILLE_DOTS_25 XK_braille_dots_25
    #define PB_KEY_BRAILLE_DOTS_125 XK_braille_dots_125
    #define PB_KEY_BRAILLE_DOTS_35 XK_braille_dots_35
    #define PB_KEY_BRAILLE_DOTS_135 XK_braille_dots_135
    #define PB_KEY_BRAILLE_DOTS_235 XK_braille_dots_235
    #define PB_KEY_BRAILLE_DOTS_1235 XK_braille_dots_1235
    #define PB_KEY_BRAILLE_DOTS_45 XK_braille_dots_45
    #define PB_KEY_BRAILLE_DOTS_145 XK_braille_dots_145
    #define PB_KEY_BRAILLE_DOTS_245 XK_braille_dots_245
    #define PB_KEY_BRAILLE_DOTS_1245 XK_braille_dots_1245
    #define PB_KEY_BRAILLE_DOTS_345 XK_braille_dots_345
    #define PB_KEY_BRAILLE_DOTS_1345 XK_braille_dots_1345
    #define PB_KEY_BRAILLE_DOTS_2345 XK_braille_dots_2345
    #define PB_KEY_BRAILLE_DOTS_12345 XK_braille_dots_12345
    #define PB_KEY_BRAILLE_DOTS_6 XK_braille_dots_6
    #define PB_KEY_BRAILLE_DOTS_16 XK_braille_dots_16
    #define PB_KEY_BRAILLE_DOTS_26 XK_braille_dots_26
    #define PB_KEY_BRAILLE_DOTS_126 XK_braille_dots_126
    #define PB_KEY_BRAILLE_DOTS_36 XK_braille_dots_36
    #define PB_KEY_BRAILLE_DOTS_136 XK_braille_dots_136
    #define PB_KEY_BRAILLE_DOTS_236 XK_braille_dots_236
    #define PB_KEY_BRAILLE_DOTS_1236 XK_braille_dots_1236
    #define PB_KEY_BRAILLE_DOTS_46 XK_braille_dots_46
    #define PB_KEY_BRAILLE_DOTS_146 XK_braille_dots_146
    #define PB_KEY_BRAILLE_DOTS_246 XK_braille_dots_246
    #define PB_KEY_BRAILLE_DOTS_1246 XK_braille_dots_1246
    #define PB_KEY_BRAILLE_DOTS_346 XK_braille_dots_346
    #define PB_KEY_BRAILLE_DOTS_1346 XK_braille_dots_1346
    #define PB_KEY_BRAILLE_DOTS_2346 XK_braille_dots_2346
    #define PB_KEY_BRAILLE_DOTS_12346 XK_braille_dots_12346
    #define PB_KEY_BRAILLE_DOTS_56 XK_braille_dots_56
    #define PB_KEY_BRAILLE_DOTS_156 XK_braille_dots_156
    #define PB_KEY_BRAILLE_DOTS_256 XK_braille_dots_256
    #define PB_KEY_BRAILLE_DOTS_1256 XK_braille_dots_1256
    #define PB_KEY_BRAILLE_DOTS_356 XK_braille_dots_356
    #define PB_KEY_BRAILLE_DOTS_1356 XK_braille_dots_1356
    #define PB_KEY_BRAILLE_DOTS_2356 XK_braille_dots_2356
    #define PB_KEY_BRAILLE_DOTS_12356 XK_braille_dots_12356
    #define PB_KEY_BRAILLE_DOTS_456 XK_braille_dots_456
    #define PB_KEY_BRAILLE_DOTS_1456 XK_braille_dots_1456
    #define PB_KEY_BRAILLE_DOTS_2456 XK_braille_dots_2456
    #define PB_KEY_BRAILLE_DOTS_12456 XK_braille_dots_12456
    #define PB_KEY_BRAILLE_DOTS_3456 XK_braille_dots_3456
    #define PB_KEY_BRAILLE_DOTS_13456 XK_braille_dots_13456
    #define PB_KEY_BRAILLE_DOTS_23456 XK_braille_dots_23456
    #define PB_KEY_BRAILLE_DOTS_123456 XK_braille_dots_123456
    #define PB_KEY_BRAILLE_DOTS_7 XK_braille_dots_7
    #define PB_KEY_BRAILLE_DOTS_17 XK_braille_dots_17
    #define PB_KEY_BRAILLE_DOTS_27 XK_braille_dots_27
    #define PB_KEY_BRAILLE_DOTS_127 XK_braille_dots_127
    #define PB_KEY_BRAILLE_DOTS_37 XK_braille_dots_37
    #define PB_KEY_BRAILLE_DOTS_137 XK_braille_dots_137
    #define PB_KEY_BRAILLE_DOTS_237 XK_braille_dots_237
    #define PB_KEY_BRAILLE_DOTS_1237 XK_braille_dots_1237
    #define PB_KEY_BRAILLE_DOTS_47 XK_braille_dots_47
    #define PB_KEY_BRAILLE_DOTS_147 XK_braille_dots_147
    #define PB_KEY_BRAILLE_DOTS_247 XK_braille_dots_247
    #define PB_KEY_BRAILLE_DOTS_1247 XK_braille_dots_1247
    #define PB_KEY_BRAILLE_DOTS_347 XK_braille_dots_347
    #define PB_KEY_BRAILLE_DOTS_1347 XK_braille_dots_1347
    #define PB_KEY_BRAILLE_DOTS_2347 XK_braille_dots_2347
    #define PB_KEY_BRAILLE_DOTS_12347 XK_braille_dots_12347
    #define PB_KEY_BRAILLE_DOTS_57 XK_braille_dots_57
    #define PB_KEY_BRAILLE_DOTS_157 XK_braille_dots_157
    #define PB_KEY_BRAILLE_DOTS_257 XK_braille_dots_257
    #define PB_KEY_BRAILLE_DOTS_1257 XK_braille_dots_1257
    #define PB_KEY_BRAILLE_DOTS_357 XK_braille_dots_357
    #define PB_KEY_BRAILLE_DOTS_1357 XK_braille_dots_1357
    #define PB_KEY_BRAILLE_DOTS_2357 XK_braille_dots_2357
    #define PB_KEY_BRAILLE_DOTS_12357 XK_braille_dots_12357
    #define PB_KEY_BRAILLE_DOTS_457 XK_braille_dots_457
    #define PB_KEY_BRAILLE_DOTS_1457 XK_braille_dots_1457
    #define PB_KEY_BRAILLE_DOTS_2457 XK_braille_dots_2457
    #define PB_KEY_BRAILLE_DOTS_12457 XK_braille_dots_12457
    #define PB_KEY_BRAILLE_DOTS_3457 XK_braille_dots_3457
    #define PB_KEY_BRAILLE_DOTS_13457 XK_braille_dots_13457
    #define PB_KEY_BRAILLE_DOTS_23457 XK_braille_dots_23457
    #define PB_KEY_BRAILLE_DOTS_123457 XK_braille_dots_123457
    #define PB_KEY_BRAILLE_DOTS_67 XK_braille_dots_67
    #define PB_KEY_BRAILLE_DOTS_167 XK_braille_dots_167
    #define PB_KEY_BRAILLE_DOTS_267 XK_braille_dots_267
    #define PB_KEY_BRAILLE_DOTS_1267 XK_braille_dots_1267
    #define PB_KEY_BRAILLE_DOTS_367 XK_braille_dots_367
    #define PB_KEY_BRAILLE_DOTS_1367 XK_braille_dots_1367
    #define PB_KEY_BRAILLE_DOTS_2367 XK_braille_dots_2367
    #define PB_KEY_BRAILLE_DOTS_12367 XK_braille_dots_12367
    #define PB_KEY_BRAILLE_DOTS_467 XK_braille_dots_467
    #define PB_KEY_BRAILLE_DOTS_1467 XK_braille_dots_1467
    #define PB_KEY_BRAILLE_DOTS_2467 XK_braille_dots_2467
    #define PB_KEY_BRAILLE_DOTS_12467 XK_braille_dots_12467
    #define PB_KEY_BRAILLE_DOTS_3467 XK_braille_dots_3467
    #define PB_KEY_BRAILLE_DOTS_13467 XK_braille_dots_13467
    #define PB_KEY_BRAILLE_DOTS_23467 XK_braille_dots_23467
    #define PB_KEY_BRAILLE_DOTS_123467 XK_braille_dots_123467
    #define PB_KEY_BRAILLE_DOTS_567 XK_braille_dots_567
    #define PB_KEY_BRAILLE_DOTS_1567 XK_braille_dots_1567
    #define PB_KEY_BRAILLE_DOTS_2567 XK_braille_dots_2567
    #define PB_KEY_BRAILLE_DOTS_12567 XK_braille_dots_12567
    #define PB_KEY_BRAILLE_DOTS_3567 XK_braille_dots_3567
    #define PB_KEY_BRAILLE_DOTS_13567 XK_braille_dots_13567
    #define PB_KEY_BRAILLE_DOTS_23567 XK_braille_dots_23567
    #define PB_KEY_BRAILLE_DOTS_123567 XK_braille_dots_123567
    #define PB_KEY_BRAILLE_DOTS_4567 XK_braille_dots_4567
    #define PB_KEY_BRAILLE_DOTS_14567 XK_braille_dots_14567
    #define PB_KEY_BRAILLE_DOTS_24567 XK_braille_dots_24567
    #define PB_KEY_BRAILLE_DOTS_124567 XK_braille_dots_124567
    #define PB_KEY_BRAILLE_DOTS_34567 XK_braille_dots_34567
    #define PB_KEY_BRAILLE_DOTS_134567 XK_braille_dots_134567
    #define PB_KEY_BRAILLE_DOTS_234567 XK_braille_dots_234567
    #define PB_KEY_BRAILLE_DOTS_1234567 XK_braille_dots_1234567
    #define PB_KEY_BRAILLE_DOTS_8 XK_braille_dots_8
    #define PB_KEY_BRAILLE_DOTS_18 XK_braille_dots_18
    #define PB_KEY_BRAILLE_DOTS_28 XK_braille_dots_28
    #define PB_KEY_BRAILLE_DOTS_128 XK_braille_dots_128
    #define PB_KEY_BRAILLE_DOTS_38 XK_braille_dots_38
    #define PB_KEY_BRAILLE_DOTS_138 XK_braille_dots_138
    #define PB_KEY_BRAILLE_DOTS_238 XK_braille_dots_238
    #define PB_KEY_BRAILLE_DOTS_1238 XK_braille_dots_1238
    #define PB_KEY_BRAILLE_DOTS_48 XK_braille_dots_48
    #define PB_KEY_BRAILLE_DOTS_148 XK_braille_dots_148
    #define PB_KEY_BRAILLE_DOTS_248 XK_braille_dots_248
    #define PB_KEY_BRAILLE_DOTS_1248 XK_braille_dots_1248
    #define PB_KEY_BRAILLE_DOTS_348 XK_braille_dots_348
    #define PB_KEY_BRAILLE_DOTS_1348 XK_braille_dots_1348
    #define PB_KEY_BRAILLE_DOTS_2348 XK_braille_dots_2348
    #define PB_KEY_BRAILLE_DOTS_12348 XK_braille_dots_12348
    #define PB_KEY_BRAILLE_DOTS_58 XK_braille_dots_58
    #define PB_KEY_BRAILLE_DOTS_158 XK_braille_dots_158
    #define PB_KEY_BRAILLE_DOTS_258 XK_braille_dots_258
    #define PB_KEY_BRAILLE_DOTS_1258 XK_braille_dots_1258
    #define PB_KEY_BRAILLE_DOTS_358 XK_braille_dots_358
    #define PB_KEY_BRAILLE_DOTS_1358 XK_braille_dots_1358
    #define PB_KEY_BRAILLE_DOTS_2358 XK_braille_dots_2358
    #define PB_KEY_BRAILLE_DOTS_12358 XK_braille_dots_12358
    #define PB_KEY_BRAILLE_DOTS_458 XK_braille_dots_458
    #define PB_KEY_BRAILLE_DOTS_1458 XK_braille_dots_1458
    #define PB_KEY_BRAILLE_DOTS_2458 XK_braille_dots_2458
    #define PB_KEY_BRAILLE_DOTS_12458 XK_braille_dots_12458
    #define PB_KEY_BRAILLE_DOTS_3458 XK_braille_dots_3458
    #define PB_KEY_BRAILLE_DOTS_13458 XK_braille_dots_13458
    #define PB_KEY_BRAILLE_DOTS_23458 XK_braille_dots_23458
    #define PB_KEY_BRAILLE_DOTS_123458 XK_braille_dots_123458
    #define PB_KEY_BRAILLE_DOTS_68 XK_braille_dots_68
    #define PB_KEY_BRAILLE_DOTS_168 XK_braille_dots_168
    #define PB_KEY_BRAILLE_DOTS_268 XK_braille_dots_268
    #define PB_KEY_BRAILLE_DOTS_1268 XK_braille_dots_1268
    #define PB_KEY_BRAILLE_DOTS_368 XK_braille_dots_368
    #define PB_KEY_BRAILLE_DOTS_1368 XK_braille_dots_1368
    #define PB_KEY_BRAILLE_DOTS_2368 XK_braille_dots_2368
    #define PB_KEY_BRAILLE_DOTS_12368 XK_braille_dots_12368
    #define PB_KEY_BRAILLE_DOTS_468 XK_braille_dots_468
    #define PB_KEY_BRAILLE_DOTS_1468 XK_braille_dots_1468
    #define PB_KEY_BRAILLE_DOTS_2468 XK_braille_dots_2468
    #define PB_KEY_BRAILLE_DOTS_12468 XK_braille_dots_12468
    #define PB_KEY_BRAILLE_DOTS_3468 XK_braille_dots_3468
    #define PB_KEY_BRAILLE_DOTS_13468 XK_braille_dots_13468
    #define PB_KEY_BRAILLE_DOTS_23468 XK_braille_dots_23468
    #define PB_KEY_BRAILLE_DOTS_123468 XK_braille_dots_123468
    #define PB_KEY_BRAILLE_DOTS_568 XK_braille_dots_568
    #define PB_KEY_BRAILLE_DOTS_1568 XK_braille_dots_1568
    #define PB_KEY_BRAILLE_DOTS_2568 XK_braille_dots_2568
    #define PB_KEY_BRAILLE_DOTS_12568 XK_braille_dots_12568
    #define PB_KEY_BRAILLE_DOTS_3568 XK_braille_dots_3568
    #define PB_KEY_BRAILLE_DOTS_13568 XK_braille_dots_13568
    #define PB_KEY_BRAILLE_DOTS_23568 XK_braille_dots_23568
    #define PB_KEY_BRAILLE_DOTS_123568 XK_braille_dots_123568
    #define PB_KEY_BRAILLE_DOTS_4568 XK_braille_dots_4568
    #define PB_KEY_BRAILLE_DOTS_14568 XK_braille_dots_14568
    #define PB_KEY_BRAILLE_DOTS_24568 XK_braille_dots_24568
    #define PB_KEY_BRAILLE_DOTS_124568 XK_braille_dots_124568
    #define PB_KEY_BRAILLE_DOTS_34568 XK_braille_dots_34568
    #define PB_KEY_BRAILLE_DOTS_134568 XK_braille_dots_134568
    #define PB_KEY_BRAILLE_DOTS_234568 XK_braille_dots_234568
    #define PB_KEY_BRAILLE_DOTS_1234568 XK_braille_dots_1234568
    #define PB_KEY_BRAILLE_DOTS_78 XK_braille_dots_78
    #define PB_KEY_BRAILLE_DOTS_178 XK_braille_dots_178
    #define PB_KEY_BRAILLE_DOTS_278 XK_braille_dots_278
    #define PB_KEY_BRAILLE_DOTS_1278 XK_braille_dots_1278
    #define PB_KEY_BRAILLE_DOTS_378 XK_braille_dots_378
    #define PB_KEY_BRAILLE_DOTS_1378 XK_braille_dots_1378
    #define PB_KEY_BRAILLE_DOTS_2378 XK_braille_dots_2378
    #define PB_KEY_BRAILLE_DOTS_12378 XK_braille_dots_12378
    #define PB_KEY_BRAILLE_DOTS_478 XK_braille_dots_478
    #define PB_KEY_BRAILLE_DOTS_1478 XK_braille_dots_1478
    #define PB_KEY_BRAILLE_DOTS_2478 XK_braille_dots_2478
    #define PB_KEY_BRAILLE_DOTS_12478 XK_braille_dots_12478
    #define PB_KEY_BRAILLE_DOTS_3478 XK_braille_dots_3478
    #define PB_KEY_BRAILLE_DOTS_13478 XK_braille_dots_13478
    #define PB_KEY_BRAILLE_DOTS_23478 XK_braille_dots_23478
    #define PB_KEY_BRAILLE_DOTS_123478 XK_braille_dots_123478
    #define PB_KEY_BRAILLE_DOTS_578 XK_braille_dots_578
    #define PB_KEY_BRAILLE_DOTS_1578 XK_braille_dots_1578
    #define PB_KEY_BRAILLE_DOTS_2578 XK_braille_dots_2578
    #define PB_KEY_BRAILLE_DOTS_12578 XK_braille_dots_12578
    #define PB_KEY_BRAILLE_DOTS_3578 XK_braille_dots_3578
    #define PB_KEY_BRAILLE_DOTS_13578 XK_braille_dots_13578
    #define PB_KEY_BRAILLE_DOTS_23578 XK_braille_dots_23578
    #define PB_KEY_BRAILLE_DOTS_123578 XK_braille_dots_123578
    #define PB_KEY_BRAILLE_DOTS_4578 XK_braille_dots_4578
    #define PB_KEY_BRAILLE_DOTS_14578 XK_braille_dots_14578
    #define PB_KEY_BRAILLE_DOTS_24578 XK_braille_dots_24578
    #define PB_KEY_BRAILLE_DOTS_124578 XK_braille_dots_124578
    #define PB_KEY_BRAILLE_DOTS_34578 XK_braille_dots_34578
    #define PB_KEY_BRAILLE_DOTS_134578 XK_braille_dots_134578
    #define PB_KEY_BRAILLE_DOTS_234578 XK_braille_dots_234578
    #define PB_KEY_BRAILLE_DOTS_1234578 XK_braille_dots_1234578
    #define PB_KEY_BRAILLE_DOTS_678 XK_braille_dots_678
    #define PB_KEY_BRAILLE_DOTS_1678 XK_braille_dots_1678
    #define PB_KEY_BRAILLE_DOTS_2678 XK_braille_dots_2678
    #define PB_KEY_BRAILLE_DOTS_12678 XK_braille_dots_12678
    #define PB_KEY_BRAILLE_DOTS_3678 XK_braille_dots_3678
    #define PB_KEY_BRAILLE_DOTS_13678 XK_braille_dots_13678
    #define PB_KEY_BRAILLE_DOTS_23678 XK_braille_dots_23678
    #define PB_KEY_BRAILLE_DOTS_123678 XK_braille_dots_123678
    #define PB_KEY_BRAILLE_DOTS_4678 XK_braille_dots_4678
    #define PB_KEY_BRAILLE_DOTS_14678 XK_braille_dots_14678
    #define PB_KEY_BRAILLE_DOTS_24678 XK_braille_dots_24678
    #define PB_KEY_BRAILLE_DOTS_124678 XK_braille_dots_124678
    #define PB_KEY_BRAILLE_DOTS_34678 XK_braille_dots_34678
    #define PB_KEY_BRAILLE_DOTS_134678 XK_braille_dots_134678
    #define PB_KEY_BRAILLE_DOTS_234678 XK_braille_dots_234678
    #define PB_KEY_BRAILLE_DOTS_1234678 XK_braille_dots_1234678
    #define PB_KEY_BRAILLE_DOTS_5678 XK_braille_dots_5678
    #define PB_KEY_BRAILLE_DOTS_15678 XK_braille_dots_15678
    #define PB_KEY_BRAILLE_DOTS_25678 XK_braille_dots_25678
    #define PB_KEY_BRAILLE_DOTS_125678 XK_braille_dots_125678
    #define PB_KEY_BRAILLE_DOTS_35678 XK_braille_dots_35678
    #define PB_KEY_BRAILLE_DOTS_135678 XK_braille_dots_135678
    #define PB_KEY_BRAILLE_DOTS_235678 XK_braille_dots_235678
    #define PB_KEY_BRAILLE_DOTS_1235678 XK_braille_dots_1235678
    #define PB_KEY_BRAILLE_DOTS_45678 XK_braille_dots_45678
    #define PB_KEY_BRAILLE_DOTS_145678 XK_braille_dots_145678
    #define PB_KEY_BRAILLE_DOTS_245678 XK_braille_dots_245678
    #define PB_KEY_BRAILLE_DOTS_1245678 XK_braille_dots_1245678
    #define PB_KEY_BRAILLE_DOTS_345678 XK_braille_dots_345678
    #define PB_KEY_BRAILLE_DOTS_1345678 XK_braille_dots_1345678
    #define PB_KEY_BRAILLE_DOTS_2345678 XK_braille_dots_2345678
    #define PB_KEY_BRAILLE_DOTS_12345678 XK_braille_dots_12345678
    #endif

    /*
    *SINHALASinhala
    *HTTP://WWW.NONGNU.ORG/SINHALA/DOC/TRANSLITERATION/SINHALA-TRANSLITERATION_6.HTMLhttp://www.nongnu.org/sinhala/doc/transliteration/sinhala-transliteration_6.html
    */

    #ifdef XK_SINHALA
    #define PB_KEY_SINH_NG XK_Sinh_ng
    #define PB_KEY_SINH_H2 XK_Sinh_h2
    #define PB_KEY_SINH_A XK_Sinh_a
    #define PB_KEY_SINH_AA XK_Sinh_aa
    #define PB_KEY_SINH_AE XK_Sinh_ae
    #define PB_KEY_SINH_AEE XK_Sinh_aee
    #define PB_KEY_SINH_I XK_Sinh_i
    #define PB_KEY_SINH_II XK_Sinh_ii
    #define PB_KEY_SINH_U XK_Sinh_u
    #define PB_KEY_SINH_UU XK_Sinh_uu
    #define PB_KEY_SINH_RI XK_Sinh_ri
    #define PB_KEY_SINH_RII XK_Sinh_rii
    #define PB_KEY_SINH_LU XK_Sinh_lu
    #define PB_KEY_SINH_LUU XK_Sinh_luu
    #define PB_KEY_SINH_E XK_Sinh_e
    #define PB_KEY_SINH_EE XK_Sinh_ee
    #define PB_KEY_SINH_AI XK_Sinh_ai
    #define PB_KEY_SINH_O XK_Sinh_o
    #define PB_KEY_SINH_OO XK_Sinh_oo
    #define PB_KEY_SINH_AU XK_Sinh_au
    #define PB_KEY_SINH_KA XK_Sinh_ka
    #define PB_KEY_SINH_KHA XK_Sinh_kha
    #define PB_KEY_SINH_GA XK_Sinh_ga
    #define PB_KEY_SINH_GHA XK_Sinh_gha
    #define PB_KEY_SINH_NG2 XK_Sinh_ng2
    #define PB_KEY_SINH_NGA XK_Sinh_nga
    #define PB_KEY_SINH_CA XK_Sinh_ca
    #define PB_KEY_SINH_CHA XK_Sinh_cha
    #define PB_KEY_SINH_JA XK_Sinh_ja
    #define PB_KEY_SINH_JHA XK_Sinh_jha
    #define PB_KEY_SINH_NYA XK_Sinh_nya
    #define PB_KEY_SINH_JNYA XK_Sinh_jnya
    #define PB_KEY_SINH_NJA XK_Sinh_nja
    #define PB_KEY_SINH_TTA XK_Sinh_tta
    #define PB_KEY_SINH_TTHA XK_Sinh_ttha
    #define PB_KEY_SINH_DDA XK_Sinh_dda
    #define PB_KEY_SINH_DDHA XK_Sinh_ddha
    #define PB_KEY_SINH_NNA XK_Sinh_nna
    #define PB_KEY_SINH_NDDA XK_Sinh_ndda
    #define PB_KEY_SINH_THA XK_Sinh_tha
    #define PB_KEY_SINH_THHA XK_Sinh_thha
    #define PB_KEY_SINH_DHA XK_Sinh_dha
    #define PB_KEY_SINH_DHHA XK_Sinh_dhha
    #define PB_KEY_SINH_NA XK_Sinh_na
    #define PB_KEY_SINH_NDHA XK_Sinh_ndha
    #define PB_KEY_SINH_PA XK_Sinh_pa
    #define PB_KEY_SINH_PHA XK_Sinh_pha
    #define PB_KEY_SINH_BA XK_Sinh_ba
    #define PB_KEY_SINH_BHA XK_Sinh_bha
    #define PB_KEY_SINH_MA XK_Sinh_ma
    #define PB_KEY_SINH_MBA XK_Sinh_mba
    #define PB_KEY_SINH_YA XK_Sinh_ya
    #define PB_KEY_SINH_RA XK_Sinh_ra
    #define PB_KEY_SINH_LA XK_Sinh_la
    #define PB_KEY_SINH_VA XK_Sinh_va
    #define PB_KEY_SINH_SHA XK_Sinh_sha
    #define PB_KEY_SINH_SSHA XK_Sinh_ssha
    #define PB_KEY_SINH_SA XK_Sinh_sa
    #define PB_KEY_SINH_HA XK_Sinh_ha
    #define PB_KEY_SINH_LLA XK_Sinh_lla
    #define PB_KEY_SINH_FA XK_Sinh_fa
    #define PB_KEY_SINH_AL XK_Sinh_al
    #define PB_KEY_SINH_AA2 XK_Sinh_aa2
    #define PB_KEY_SINH_AE2 XK_Sinh_ae2
    #define PB_KEY_SINH_AEE2 XK_Sinh_aee2
    #define PB_KEY_SINH_I2 XK_Sinh_i2
    #define PB_KEY_SINH_II2 XK_Sinh_ii2
    #define PB_KEY_SINH_U2 XK_Sinh_u2
    #define PB_KEY_SINH_UU2 XK_Sinh_uu2
    #define PB_KEY_SINH_RU2 XK_Sinh_ru2
    #define PB_KEY_SINH_E2 XK_Sinh_e2
    #define PB_KEY_SINH_EE2 XK_Sinh_ee2
    #define PB_KEY_SINH_AI2 XK_Sinh_ai2
    #define PB_KEY_SINH_O2 XK_Sinh_o2
    #define PB_KEY_SINH_OO2 XK_Sinh_oo2
    #define PB_KEY_SINH_AU2 XK_Sinh_au2
    #define PB_KEY_SINH_LU2 XK_Sinh_lu2
    #define PB_KEY_SINH_RUU2 XK_Sinh_ruu2
    #define PB_KEY_SINH_LUU2 XK_Sinh_luu2
    #define PB_KEY_SINH_KUNDDALIYA XK_Sinh_kunddaliya
#endif
#elif defined(TARGET_PLATFORM_WINDOWS)
    #include <Windows.h>
    #define PB_KEY_BACKSPACE VK_BACK
    #define PB_KEY_TAB VK_TAB
    #define PB_KEY_ENTER VK_RETURN
    #define PB_KEY_RETURN VK_RETURN
    #define PB_KEY_KP_RETURN VK_RETURN
    #define PB_KEY_KP_ENTER VK_RETURN
    #define PB_KEY_ISO_RETURN VK_RETURN
    #define PB_KEY_ISO_ENTER VK_RETURN
    #define PB_KEY_CLEAR VK_CLEAR
    #define PB_KEY_SHIFT_L VK_LSHIFT
    #define PB_KEY_SHIFT_R VK_RSHIFT
    #define PB_KEY_CONTROL_L VK_LCONTROL
    #define PB_KEY_CONTROL_R VK_RCONTROL
    #define PB_KEY_ALT VK_MENU
    #define PB_KEY_PAUSE VK_PAUSE
    #define PB_KEY_CAPSLOCK VK_CAPITAL
    #define PB_KEY_ESCAPE VK_ESCAPE
    #define PB_KEY_SPACE VK_SPACE
    #define PB_KEY_PAGEUP VK_PRIOR
    #define PB_KEY_PAGEDOWN VK_NEXT
    #define PB_KEY_END VK_END
    #define PB_KEY_HOME VK_HOME
    #define PB_KEY_LEFT VK_LEFT
    #define PB_KEY_UP VK_UP
    #define PB_KEY_RIGHT VK_RIGHT
    #define PB_KEY_DOWN VK_DOWN
    #define PB_KEY_PRINT VK_SNAPSHOT
    #define PB_KEY_INSERT VK_INSERT
    #define PB_KEY_DELETE VK_DELETE
    #define PB_KEY_0 0x30
    #define PB_KEY_1 0x31
    #define PB_KEY_2 0x32
    #define PB_KEY_3 0x33
    #define PB_KEY_4 0x34
    #define PB_KEY_5 0x35
    #define PB_KEY_6 0x36
    #define PB_KEY_7 0x37
    #define PB_KEY_8 0x38
    #define PB_KEY_9 0x39
    #define PB_KEY_A 0x41
    #define PB_KEY_B 0x42
    #define PB_KEY_C 0x43
    #define PB_KEY_D 0x44
    #define PB_KEY_E 0x45
    #define PB_KEY_F 0x46
    #define PB_KEY_G 0x47
    #define PB_KEY_H 0x48
    #define PB_KEY_I 0x49
    #define PB_KEY_J 0x4A
    #define PB_KEY_K 0x4B
    #define PB_KEY_L 0x4C
    #define PB_KEY_M 0x4D
    #define PB_KEY_N 0x4E
    #define PB_KEY_O 0x4F
    #define PB_KEY_P 0x50
    #define PB_KEY_Q 0x51
    #define PB_KEY_R 0x52
    #define PB_KEY_S 0x53
    #define PB_KEY_T 0x54
    #define PB_KEY_U 0x55
    #define PB_KEY_V 0x56
    #define PB_KEY_W 0x57
    #define PB_KEY_X 0x58
    #define PB_KEY_Y 0x59
    #define PB_KEY_Z 0x5A
    #define PB_KEY_HANGUL VK_HANGUL
    #define PB_KEY_HANGUL_HANJA VK_HANJA
    #define PB_KEY_KANA VK_KANA
    #define PB_KEY_KANJI VK_KANJI
    #define PB_KEY_JUNJA VK_JUNJA
    #define PB_KEY_FINAL VK_FINAL
    #define PB_KEY_SUPER_L VK_LWIN
    #define PB_KEY_SUPER_R VK_RWIN
    #define PB_KEY_NUMPAD0 VK_NUMPAD0
    #define PB_KEY_KP_0 VK_NUMPAD0
    #define PB_KEY_KP_1 VK_NUMPAD1
    #define PB_KEY_KP_2 VK_NUMPAD2
    #define PB_KEY_KP_3 VK_NUMPAD3
    #define PB_KEY_KP_4 VK_NUMPAD4
    #define PB_KEY_KP_5 VK_NUMPAD5
    #define PB_KEY_KP_6 VK_NUMPAD6
    #define PB_KEY_KP_7 VK_NUMPAD7
    #define PB_KEY_KP_8 VK_NUMPAD8
    #define PB_KEY_KP_9 VK_NUMPAD9
    #define PB_KEY_MULTIPLY VK_MULTIPLY
    #define PB_KEY_KP_ADD VK_ADD
    #define PB_KEY_KP_SEPARATOR VK_SEPARATOR
    #define PB_KEY_KP_SUBTRACT VK_SUBTRACT
    #define PB_KEY_KP_DECIMALPOINT VK_DECIMAL
    #define PB_KEY_KP_DIVIDE VK_DIVIDE
    #define PB_KEY_F1 VK_F1
    #define PB_KEY_F2 VK_F2
    #define PB_KEY_F3 VK_F3
    #define PB_KEY_F4 VK_F4
    #define PB_KEY_F5 VK_F5
    #define PB_KEY_F6 VK_F6
    #define PB_KEY_F7 VK_F7
    #define PB_KEY_F8 VK_F8
    #define PB_KEY_F9 VK_F9
    #define PB_KEY_F10 VK_F10
    #define PB_KEY_F11 VK_F11
    #define PB_KEY_F12 VK_F12
    #define PB_KEY_F13 VK_F13
    #define PB_KEY_F14 VK_F14
    #define PB_KEY_F15 VK_F15
    #define PB_KEY_F16 VK_F16
    #define PB_KEY_F17 VK_F17
    #define PB_KEY_F18 VK_F18
    #define PB_KEY_F19 VK_F19
    #define PB_KEY_F20 VK_F20
    #define PB_KEY_F21 VK_F21
    #define PB_KEY_F22 VK_F22
    #define PB_KEY_F23 VK_F23
    #define PB_KEY_F24 VK_F24
    #define PB_KEY_MODE_SWITCH VK_MODECHANGE
    #define PB_KEY_SELECT VK_SELECT
    #define PB_KEY_EXECUTE VK_EXECUTE
    #define PB_KEY_HELP VK_HELP
    #define PB_KEY_NUMLOCK VK_NUMLOCK
    #define PB_KEY_SCROLL VK_SCROLL


    //Windows exclusives
    #define PB_KEY_IME_ON VK_IME_ON
    #define PB_KEY_IME_OFF VK_IME_OFF
    //#define PB_KEY_SHIFT VK_SHIFT
    //#define PB_KEY_CONTROL VK_CONTROL
    #define PB_KEY_APPS VK_APPS
    #define PB_KEY_CONVERT VK_CONVERT
    #define PB_KEY_NONCONVERT VK_NONCONVERT
    #define PB_KEY_ACCEPT VK_ACCEPT
    #define PB_KEY_SLEEP VK_SLEEP
    #if(_WIN32_WINNT >= 0x0604) //reserved
        #define PB_KEY_NAVIGATION_VIEW VK_NAVIGATION_VIEW
        #define PB_KEY_NAVIGATION_MENU VK_NAVIGATION_MENU
        #define PB_KEY_NAVIGATION_UP VK_NAVIGATION_UP
        #define PB_KEY_NAVIGATION_DOWN VK_NAVIGATION_DOWN
        #define PB_KEY_NAVIGATION_LEFT VK_NAVIGATION_LEFT
        #define PB_KEY_NAVIGATION_RIGHT VK_NAVIGATION_RIGHT
        #define PB_KEY_NAVIGATION_ACCEPT VK_NAVIGATION_ACCEPT
        #define PB_KEY_NAVIGATION_CANCEL VK_NAVIGATION_CANCEL
    #endif // _WIN32_WINNT >= 0x0604
    #if(_WIN32_WINNT >= 0x0500)
        #define PB_KEY_BROWSER_BACK VK_BROWSER_BACK
        #define PB_KEY_BROWSER_FORWARD VK_BROWSER_FORWARD
        #define PB_KEY_BROWSER_REFRESH VK_BROWSER_REFRESH
        #define PB_KEY_BROWSER_STOP VK_BROWSER_STOP
        #define PB_KEY_BROWSER_SEARCH VK_BROWSER_SEARCH
        #define PB_KEY_BROWSER_FAVORITES VK_BROWSER_FAVORITES
        #define PB_KEY_BROWSER_HOME VK_BROWSER_HOME
        #define PB_KEY_VOLUME_MUTE VK_VOLUME_MUTE
        #define PB_KEY_VOLUME_DOWN VK_VOLUME_DOWN
        #define PB_KEY_VOLUME_UP VK_VOLUME_UP
        #define PB_KEY_MEDIA_NEXT_TRACK VK_MEDIA_NEXT_TRACK
        #define PB_KEY_MEDIA_PREV_TRACK VK_MEDIA_PREV_TRACK
        #define PB_KEY_MEDIA_STOP VK_MEDIA_STOP
        #define PB_KEY_MEDIA_PLAY_PAUSE VK_MEDIA_PLAY_PAUSE
        #define PB_KEY_LAUNCH_MAIL VK_LAUNCH_MAIL
        #define PB_KEY_LAUNCH_MEDIA_SELECT VK_LAUNCH_MEDIA_SELECT
        #define PB_KEY_LAUNCH_APP1 VK_LAUNCH_APP1
        #define PB_KEY_LAUNCH_APP2 VK_LAUNCH_APP2
    #endif // _WIN32_WINNT >= 0x0500
    #define PB_KEY_LMENU VK_LMENU
    #define PB_KEY_RMENU VK_RMENU
    #define PB_KEY_OEM_NEC_EQUAL VK_OEM_NEC_EQUAL
    #define PB_KEY_OEM_FJ_JISHO VK_OEM_FJ_JISHO
    #define PB_KEY_OEM_FJ_MASSHOU VK_OEM_FJ_MASSHOU
    #define PB_KEY_OEM_FJ_TOUROKU VK_OEM_FJ_TOUROKU
    #define PB_KEY_OEM_FJ_LOYA VK_OEM_FJ_LOYA
    #define PB_KEY_OEM_FJ_ROYA VK_OEM_FJ_ROYA
    #define PB_KEY_OEM_1 VK_OEM_1           // ';:' for US
    #define PB_KEY_OEM_PLUS VK_OEM_PLUS     // '+' any country
    #define PB_KEY_OEM_COMMA VK_OEM_COMMA   // ',' any country
    #define PB_KEY_OEM_MINUS VK_OEM_MINUS   // '-' any country
    #define PB_KEY_OEM_PERIOD VK_OEM_PERIOD // '.' any country
    #define PB_KEY_OEM_2 VK_OEM_2           // '/?' for US
    #define PB_KEY_OEM_3 VK_OEM_3           // '`~' for US
    #define PB_KEY_OEM_4 VK_OEM_4           // '[{' for US
    #define PB_KEY_OEM_5 VK_OEM_5           // '\|' for US
    #define PB_KEY_OEM_6 VK_OEM_6           // ']}' for US
    #define PB_KEY_OEM_7 VK_OEM_7           // ''"' for US
    #define PB_KEY_OEM_8 VK_OEM_8           // ']' for US
    #define PB_KEY_OEM_AX VK_OEM_AX         // 'AX' key on Japanese AX kbd
    #define PB_KEY_OEM_102 VK_OEM_102       // "<>" or "\|" on RT 102-key kbd.
    #define PB_KEY_ICO_HELP VK_ICO_HELP     // Help key on ICO
    #define PB_KEY_ICO_00 VK_ICO_00         // 00 key on ICO
    #if(WINVER >= 0x0400)
        #define PB_KEY_PROCESSKEY VK_PROCESSKEY
    #endif // WINVER >= 0x0400
    #define PB_KEY_ICO_CLEAR VK_ICO_CLEAR
    #if(_WIN32_WINNT >= 0x0500)
        #define PB_KEY_PACKET VK_PACKET
    #endif // _WIN32_WINNT >= 0x0500
    #define PB_KEY_OEM_RESET VK_OEM_RESET
    #define PB_KEY_OEM_JUMP VK_OEM_JUMP
    #define PB_KEY_OEM_PA1 VK_OEM_PA1
    #define PB_KEY_OEM_PA2 VK_OEM_PA2
    #define PB_KEY_OEM_PA3 VK_OEM_PA3
    #define PB_KEY_OEM_WSCTRL VK_OEM_WSCTRL
    #define PB_KEY_OEM_CUSEL VK_OEM_CUSEL
    #define PB_KEY_OEM_ATTN VK_OEM_ATTN
    #define PB_KEY_OEM_FINISH VK_OEM_FINISH
    #define PB_KEY_OEM_COPY VK_OEM_COPY
    #define PB_KEY_OEM_AUTO VK_OEM_AUTO
    #define PB_KEY_OEM_ENLW VK_OEM_ENLW
    #define PB_KEY_OEM_BACKTAB VK_OEM_BACKTAB
    #define PB_KEY_ATTN VK_ATTN
    #define PB_KEY_CRSEL VK_CRSEL
    #define PB_KEY_EXSEL VK_EXSEL
    #define PB_KEY_EREOF VK_EREOF
    #define PB_KEY_PLAY VK_PLAY
    #define PB_KEY_ZOOM VK_ZOOM
    #define PB_KEY_NONAME VK_NONAME
    #define PB_KEY_PA1 VK_PA1
    #define PB_KEY_OEM_CLEAR VK_OEM_CLEAR

/*


#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    // NOT contiguous with L & RBUTTON 

#if(_WIN32_WINNT >= 0x0500)
#define VK_XBUTTON1       0x05    // NOT contiguous with L & RBUTTON
#define VK_XBUTTON2       0x06    // NOT contiguous with L & RBUTTON
#endif // _WIN32_WINNT >= 0x0500



 // 0xC3 - 0xDA : Gamepad input
#if(_WIN32_WINNT >= 0x0604)
#define VK_GAMEPAD_A                         0xC3 // reserved
#define VK_GAMEPAD_B                         0xC4 // reserved
#define VK_GAMEPAD_X                         0xC5 // reserved
#define VK_GAMEPAD_Y                         0xC6 // reserved
#define VK_GAMEPAD_RIGHT_SHOULDER            0xC7 // reserved
#define VK_GAMEPAD_LEFT_SHOULDER             0xC8 // reserved
#define VK_GAMEPAD_LEFT_TRIGGER              0xC9 // reserved
#define VK_GAMEPAD_RIGHT_TRIGGER             0xCA // reserved
#define VK_GAMEPAD_DPAD_UP                   0xCB // reserved
#define VK_GAMEPAD_DPAD_DOWN                 0xCC // reserved
#define VK_GAMEPAD_DPAD_LEFT                 0xCD // reserved
#define VK_GAMEPAD_DPAD_RIGHT                0xCE // reserved
#define VK_GAMEPAD_MENU                      0xCF // reserved
#define VK_GAMEPAD_VIEW                      0xD0 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON    0xD1 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON   0xD2 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_UP        0xD3 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_DOWN      0xD4 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT     0xD5 // reserved
#define VK_GAMEPAD_LEFT_THUMBSTICK_LEFT      0xD6 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_UP       0xD7 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN     0xD8 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT    0xD9 // reserved
#define VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT     0xDA // reserved

#endif // _WIN32_WINNT >= 0x0604 

*/

#endif