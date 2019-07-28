public char * save LESSPARAMS ((constant char *s));
public VOID_POINTER ecalloc LESSPARAMS ((int count, unsigned int size));
public char * skipsp LESSPARAMS ((char *s));
public int sprefix LESSPARAMS ((char *ps, char *s, int uppercase));
public void quit LESSPARAMS ((int status));
public void raw_mode LESSPARAMS ((int on));
public void scrsize LESSPARAMS ((VOID_PARAM));
public char * special_key_str LESSPARAMS ((int key));
public void get_term LESSPARAMS ((VOID_PARAM));
public void init_mouse LESSPARAMS ((VOID_PARAM));
public void deinit_mouse LESSPARAMS ((VOID_PARAM));
public void init LESSPARAMS ((VOID_PARAM));
public void deinit LESSPARAMS ((VOID_PARAM));
public void home LESSPARAMS ((VOID_PARAM));
public void add_line LESSPARAMS ((VOID_PARAM));
public void remove_top LESSPARAMS ((int n));
public void win32_scroll_up LESSPARAMS ((int n));
public void lower_left LESSPARAMS ((VOID_PARAM));
public void line_left LESSPARAMS ((VOID_PARAM));
public void check_winch LESSPARAMS ((VOID_PARAM));
public void goto_line LESSPARAMS ((int sindex));
public void vbell LESSPARAMS ((VOID_PARAM));
public void bell LESSPARAMS ((VOID_PARAM));
public void clear LESSPARAMS ((VOID_PARAM));
public void clear_eol LESSPARAMS ((VOID_PARAM));
public void clear_bot LESSPARAMS ((VOID_PARAM));
public void at_enter LESSPARAMS ((int attr));
public void at_exit LESSPARAMS ((VOID_PARAM));
public void at_switch LESSPARAMS ((int attr));
public int is_at_equiv LESSPARAMS ((int attr1, int attr2));
public int apply_at_specials LESSPARAMS ((int attr));
public void backspace LESSPARAMS ((VOID_PARAM));
public void putbs LESSPARAMS ((VOID_PARAM));
public int win32_kbhit LESSPARAMS ((VOID_PARAM));
public char WIN32getch LESSPARAMS ((VOID_PARAM));
public void WIN32setcolors LESSPARAMS ((int fg, int bg));
public void WIN32textout LESSPARAMS ((char *text, int len));
public void match_brac LESSPARAMS ((int obrac, int cbrac, int forwdir, int n));
public void ch_ungetchar LESSPARAMS ((int c));
public void end_logfile LESSPARAMS ((VOID_PARAM));
public void sync_logfile LESSPARAMS ((VOID_PARAM));
public int ch_seek LESSPARAMS ((POSITION pos));
public int ch_end_seek LESSPARAMS ((VOID_PARAM));
public int ch_end_buffer_seek LESSPARAMS ((VOID_PARAM));
public int ch_beg_seek LESSPARAMS ((VOID_PARAM));
public POSITION ch_length LESSPARAMS ((VOID_PARAM));
public POSITION ch_tell LESSPARAMS ((VOID_PARAM));
public int ch_forw_get LESSPARAMS ((VOID_PARAM));
public int ch_back_get LESSPARAMS ((VOID_PARAM));
public void ch_setbufspace LESSPARAMS ((int bufspace));
public void ch_flush LESSPARAMS ((VOID_PARAM));
public int seekable LESSPARAMS ((int f));
public void ch_set_eof LESSPARAMS ((VOID_PARAM));
public void ch_init LESSPARAMS ((int f, int flags));
public void ch_close LESSPARAMS ((VOID_PARAM));
public int ch_getflags LESSPARAMS ((VOID_PARAM));
public void ch_dump LESSPARAMS ((VOID_PARAM));
public void setfmt LESSPARAMS ((char *s, char **fmtvarptr, int *attrptr, char *default_fmt));
public void init_charset LESSPARAMS ((VOID_PARAM));
public int binary_char LESSPARAMS ((LWCHAR c));
public int control_char LESSPARAMS ((LWCHAR c));
public char * prchar LESSPARAMS ((LWCHAR c));
public char * prutfchar LESSPARAMS ((LWCHAR ch));
public int utf_len LESSPARAMS ((unsigned char ch));
public int is_utf8_well_formed LESSPARAMS ((char *ss, int slen));
public void utf_skip_to_lead LESSPARAMS ((char **pp, char *limit));
public LWCHAR get_wchar LESSPARAMS ((constant char *p));
public void put_wchar LESSPARAMS ((char **pp, LWCHAR ch));
public LWCHAR step_char LESSPARAMS ((char **pp, signed int dir, constant char *limit));
public int is_composing_char LESSPARAMS ((LWCHAR ch));
public int is_ubin_char LESSPARAMS ((LWCHAR ch));
public int is_wide_char LESSPARAMS ((LWCHAR ch));
public int is_combining_char LESSPARAMS ((LWCHAR ch1, LWCHAR ch2));
public void cmd_reset LESSPARAMS ((VOID_PARAM));
public void clear_cmd LESSPARAMS ((VOID_PARAM));
public void cmd_putstr LESSPARAMS ((constant char *s));
public int len_cmdbuf LESSPARAMS ((VOID_PARAM));
public void set_mlist LESSPARAMS ((void *mlist, int cmdflags));
public void cmd_addhist LESSPARAMS ((struct mlist *mlist, constant char *cmd, int modified));
public void cmd_accept LESSPARAMS ((VOID_PARAM));
public int cmd_char LESSPARAMS ((int c));
public LINENUM cmd_int LESSPARAMS ((long *frac));
public char * get_cmdbuf LESSPARAMS ((VOID_PARAM));
public char * cmd_lastpattern LESSPARAMS ((VOID_PARAM));
public void init_cmdhist LESSPARAMS ((VOID_PARAM));
public void save_cmdhist LESSPARAMS ((VOID_PARAM));
public int in_mca LESSPARAMS ((VOID_PARAM));
public void dispversion LESSPARAMS ((VOID_PARAM));
public int getcc LESSPARAMS ((VOID_PARAM));
public void ungetcc LESSPARAMS ((LWCHAR c));
public void ungetsc LESSPARAMS ((char *s));
public LWCHAR peekcc LESSPARAMS ((VOID_PARAM));
public void commands LESSPARAMS ((VOID_PARAM));
public int cvt_length LESSPARAMS ((int len, int ops));
public int * cvt_alloc_chpos LESSPARAMS ((int len));
public void cvt_text LESSPARAMS ((char *odst, char *osrc, int *chpos, int *lenp, int ops));
public void expand_cmd_tables LESSPARAMS ((VOID_PARAM));
public void init_cmds LESSPARAMS ((VOID_PARAM));
public void add_fcmd_table LESSPARAMS ((char *buf, int len));
public void add_ecmd_table LESSPARAMS ((char *buf, int len));
public int fcmd_decode LESSPARAMS ((char *cmd, char **sp));
public int ecmd_decode LESSPARAMS ((char *cmd, char **sp));
public char * lgetenv LESSPARAMS ((char *var));
public int isnullenv LESSPARAMS ((char* s));
public int lesskey LESSPARAMS ((char *filename, int sysvar));
public void add_hometable LESSPARAMS ((char *envname, char *def_filename, int sysvar));
public int editchar LESSPARAMS ((int c, int flags));
public void init_textlist LESSPARAMS ((struct textlist *tlist, char *str));
public char * forw_textlist LESSPARAMS ((struct textlist *tlist, char *prev));
public char * back_textlist LESSPARAMS ((struct textlist *tlist, char *prev));
public int edit LESSPARAMS ((char *filename));
public int edit_ifile LESSPARAMS ((IFILE ifile));
public int edit_list LESSPARAMS ((char *filelist));
public int edit_first LESSPARAMS ((VOID_PARAM));
public int edit_last LESSPARAMS ((VOID_PARAM));
public int edit_next LESSPARAMS ((int n));
public int edit_prev LESSPARAMS ((int n));
public int edit_index LESSPARAMS ((int n));
public IFILE save_curr_ifile LESSPARAMS ((VOID_PARAM));
public void unsave_ifile LESSPARAMS ((IFILE save_ifile));
public void reedit_ifile LESSPARAMS ((IFILE save_ifile));
public void reopen_curr_ifile LESSPARAMS ((VOID_PARAM));
public int edit_stdin LESSPARAMS ((VOID_PARAM));
public void cat_file LESSPARAMS ((VOID_PARAM));
public void use_logfile LESSPARAMS ((char *filename));
public char * shell_unquote LESSPARAMS ((char *str));
public char * get_meta_escape LESSPARAMS ((VOID_PARAM));
public char * shell_quote LESSPARAMS ((char *s));
public char * homefile LESSPARAMS ((char *filename));
public char * fexpand LESSPARAMS ((char *s));
public char * fcomplete LESSPARAMS ((char *s));
public int bin_file LESSPARAMS ((int f));
public char * lglob LESSPARAMS ((char *filename));
public char * lrealpath LESSPARAMS ((char *path));
public char * open_altfile LESSPARAMS ((char *filename, int *pf, void **pfd));
public void close_altfile LESSPARAMS ((char *altfilename, char *filename));
public int is_dir LESSPARAMS ((char *filename));
public char * bad_file LESSPARAMS ((char *filename));
public POSITION filesize LESSPARAMS ((int f));
public char * shell_coption LESSPARAMS ((VOID_PARAM));
public char * last_component LESSPARAMS ((char *name));
public int eof_displayed LESSPARAMS ((VOID_PARAM));
public int entire_file_displayed LESSPARAMS ((VOID_PARAM));
public void squish_check LESSPARAMS ((VOID_PARAM));
public void forw LESSPARAMS ((int n, POSITION pos, int force, int only_last, int nblank));
public void back LESSPARAMS ((int n, POSITION pos, int force, int only_last));
public void forward LESSPARAMS ((int n, int force, int only_last));
public void backward LESSPARAMS ((int n, int force, int only_last));
public int get_back_scroll LESSPARAMS ((VOID_PARAM));
public int get_one_screen LESSPARAMS ((VOID_PARAM));
public void del_ifile LESSPARAMS ((IFILE h));
public IFILE next_ifile LESSPARAMS ((IFILE h));
public IFILE prev_ifile LESSPARAMS ((IFILE h));
public IFILE getoff_ifile LESSPARAMS ((IFILE ifile));
public int nifile LESSPARAMS ((VOID_PARAM));
public IFILE get_ifile LESSPARAMS ((char *filename, IFILE prev));
public char * get_filename LESSPARAMS ((IFILE ifile));
public int get_index LESSPARAMS ((IFILE ifile));
public void store_pos LESSPARAMS ((IFILE ifile, struct scrpos *scrpos));
public void get_pos LESSPARAMS ((IFILE ifile, struct scrpos *scrpos));
public void set_open LESSPARAMS ((IFILE ifile));
public int opened LESSPARAMS ((IFILE ifile));
public void hold_ifile LESSPARAMS ((IFILE ifile, int incr));
public int held_ifile LESSPARAMS ((IFILE ifile));
public void * get_filestate LESSPARAMS ((IFILE ifile));
public void set_filestate LESSPARAMS ((IFILE ifile, void *filestate));
public void set_altpipe LESSPARAMS ((IFILE ifile, void *p));
public void * get_altpipe LESSPARAMS ((IFILE ifile));
public void set_altfilename LESSPARAMS ((IFILE ifile, char *altfilename));
public char * get_altfilename LESSPARAMS ((IFILE ifile));
public void if_dump LESSPARAMS ((VOID_PARAM));
public POSITION forw_line LESSPARAMS ((POSITION curr_pos));
public POSITION back_line LESSPARAMS ((POSITION curr_pos));
public void set_attnpos LESSPARAMS ((POSITION pos));
public void jump_forw LESSPARAMS ((VOID_PARAM));
public void jump_forw_buffered LESSPARAMS ((VOID_PARAM));
public void jump_back LESSPARAMS ((LINENUM linenum));
public void repaint LESSPARAMS ((VOID_PARAM));
public void jump_percent LESSPARAMS ((int percent, long fraction));
public void jump_line_loc LESSPARAMS ((POSITION pos, int sline));
public void jump_loc LESSPARAMS ((POSITION pos, int sline));
public void init_line LESSPARAMS ((VOID_PARAM));
public int is_ascii_char LESSPARAMS ((LWCHAR ch));
public void prewind LESSPARAMS ((VOID_PARAM));
public void plinenum LESSPARAMS ((POSITION pos));
public void pshift_all LESSPARAMS ((VOID_PARAM));
public int is_ansi_end LESSPARAMS ((LWCHAR ch));
public int is_ansi_middle LESSPARAMS ((LWCHAR ch));
public void skip_ansi LESSPARAMS ((char **pp, constant char *limit));
public int pappend LESSPARAMS ((unsigned char c, POSITION pos));
public int pflushmbc LESSPARAMS ((VOID_PARAM));
public void pdone LESSPARAMS ((int endline, int chopped, int forw));
public void set_status_col LESSPARAMS ((char c));
public int gline LESSPARAMS ((int i, int *ap));
public void null_line LESSPARAMS ((VOID_PARAM));
public POSITION forw_raw_line LESSPARAMS ((POSITION curr_pos, char **linep, int *line_lenp));
public POSITION back_raw_line LESSPARAMS ((POSITION curr_pos, char **linep, int *line_lenp));
public int rrshift LESSPARAMS ((VOID_PARAM));
public void clr_linenum LESSPARAMS ((VOID_PARAM));
public void add_lnum LESSPARAMS ((LINENUM linenum, POSITION pos));
public LINENUM find_linenum LESSPARAMS ((POSITION pos));
public POSITION find_pos LESSPARAMS ((LINENUM linenum));
public LINENUM currline LESSPARAMS ((int where));
public void lsystem LESSPARAMS ((char *cmd, char *donemsg));
public int pipe_mark LESSPARAMS ((int c, char *cmd));
public int pipe_data LESSPARAMS ((char *cmd, POSITION spos, POSITION epos));
public void init_mark LESSPARAMS ((VOID_PARAM));
public int badmark LESSPARAMS ((int c));
public void setmark LESSPARAMS ((int c, int where));
public void clrmark LESSPARAMS ((int c));
public void lastmark LESSPARAMS ((VOID_PARAM));
public void gomark LESSPARAMS ((int c));
public POSITION markpos LESSPARAMS ((int c));
public char posmark LESSPARAMS ((POSITION pos));
public void unmark LESSPARAMS ((IFILE ifile));
public void mark_check_ifile LESSPARAMS ((IFILE ifile));
public void save_marks LESSPARAMS ((FILE *fout, char *hdr));
public void restore_mark LESSPARAMS ((char *line));
public void opt_o LESSPARAMS ((int type, char *s));
public void opt__O LESSPARAMS ((int type, char *s));
public void opt_j LESSPARAMS ((int type, char *s));
public void calc_jump_sline LESSPARAMS ((VOID_PARAM));
public void opt_shift LESSPARAMS ((int type, char *s));
public void calc_shift_count LESSPARAMS ((VOID_PARAM));
public void opt_k LESSPARAMS ((int type, char *s));
public void opt_t LESSPARAMS ((int type, char *s));
public void opt__T LESSPARAMS ((int type, char *s));
public void opt_p LESSPARAMS ((int type, char *s));
public void opt__P LESSPARAMS ((int type, char *s));
public void opt_b LESSPARAMS ((int type, char *s));
public void opt_i LESSPARAMS ((int type, char *s));
public void opt__V LESSPARAMS ((int type, char *s));
public void opt_D LESSPARAMS ((int type, char *s));
public void opt_x LESSPARAMS ((int type, char *s));
public void opt_quote LESSPARAMS ((int type, char *s));
public void opt_rscroll LESSPARAMS ((int type, char *s));
public void opt_query LESSPARAMS ((int type, char *s));
public void opt_mousecap LESSPARAMS ((int type, char *s));
public void opt_wheel_lines LESSPARAMS ((int type, char *s));
public int get_swindow LESSPARAMS ((VOID_PARAM));
public char * propt LESSPARAMS ((int c));
public void scan_option LESSPARAMS ((char *s));
public void toggle_option LESSPARAMS ((struct loption *o, int lower, char *s, int how_toggle));
public int opt_has_param LESSPARAMS ((struct loption *o));
public char * opt_prompt LESSPARAMS ((struct loption *o));
public int isoptpending LESSPARAMS ((VOID_PARAM));
public void nopendopt LESSPARAMS ((VOID_PARAM));
public int getnum LESSPARAMS ((char **sp, char *printopt, int *errp));
public long getfraction LESSPARAMS ((char **sp, char *printopt, int *errp));
public int get_quit_at_eof LESSPARAMS ((VOID_PARAM));
public void init_option LESSPARAMS ((VOID_PARAM));
public struct loption * findopt LESSPARAMS ((int c));
public struct loption * findopt_name LESSPARAMS ((char **p_optname, char **p_oname, int *p_err));
public int iread LESSPARAMS ((int fd, unsigned char *buf, unsigned int len));
public void intread LESSPARAMS ((VOID_PARAM));
public time_type get_time LESSPARAMS ((VOID_PARAM));
public char * errno_message LESSPARAMS ((char *filename));
public int percentage LESSPARAMS ((POSITION num, POSITION den));
public POSITION percent_pos LESSPARAMS ((POSITION pos, int percent, long fraction));
public int  os9_signal LESSPARAMS ((int type, RETSIGTYPE (*handler)()));
public void put_line LESSPARAMS ((VOID_PARAM));
public void flush LESSPARAMS ((VOID_PARAM));
public int putchr LESSPARAMS ((int c));
public void putstr LESSPARAMS ((constant char *s));
public void get_return LESSPARAMS ((VOID_PARAM));
public void error LESSPARAMS ((char *fmt, PARG *parg));
public void ierror LESSPARAMS ((char *fmt, PARG *parg));
public int query LESSPARAMS ((char *fmt, PARG *parg));
public int compile_pattern LESSPARAMS ((char *pattern, int search_type, PATTERN_TYPE *comp_pattern));
public void uncompile_pattern LESSPARAMS ((PATTERN_TYPE *pattern));
public int valid_pattern LESSPARAMS ((char *pattern));
public int is_null_pattern LESSPARAMS ((PATTERN_TYPE pattern));
public int match_pattern LESSPARAMS ((PATTERN_TYPE pattern, char *tpattern, char *line, int line_len, char **sp, char **ep, int notbol, int search_type));
public char * pattern_lib_name LESSPARAMS ((VOID_PARAM));
public POSITION position LESSPARAMS ((int sindex));
public void add_forw_pos LESSPARAMS ((POSITION pos));
public void add_back_pos LESSPARAMS ((POSITION pos));
public void pos_clear LESSPARAMS ((VOID_PARAM));
public void pos_init LESSPARAMS ((VOID_PARAM));
public int onscreen LESSPARAMS ((POSITION pos));
public int empty_screen LESSPARAMS ((VOID_PARAM));
public int empty_lines LESSPARAMS ((int s, int e));
public void get_scrpos LESSPARAMS ((struct scrpos *scrpos, int where));
public int sindex_from_sline LESSPARAMS ((int sline));
public void init_prompt LESSPARAMS ((VOID_PARAM));
public char * pr_expand LESSPARAMS ((constant char *proto, int maxwidth));
public char * eq_message LESSPARAMS ((VOID_PARAM));
public char * pr_string LESSPARAMS ((VOID_PARAM));
public char * wait_message LESSPARAMS ((VOID_PARAM));
public void init_search LESSPARAMS ((VOID_PARAM));
public void repaint_hilite LESSPARAMS ((int on));
public void clear_attn LESSPARAMS ((VOID_PARAM));
public void undo_search LESSPARAMS ((VOID_PARAM));
public void clr_hlist LESSPARAMS ((struct hilite_tree *anchor));
public void clr_hilite LESSPARAMS ((VOID_PARAM));
public void clr_filter LESSPARAMS ((VOID_PARAM));
public int is_filtered LESSPARAMS ((POSITION pos));
public POSITION next_unfiltered LESSPARAMS ((POSITION pos));
public POSITION prev_unfiltered LESSPARAMS ((POSITION pos));
public int is_hilited LESSPARAMS ((POSITION pos, POSITION epos, int nohide, int *p_matches));
public void chg_hilite LESSPARAMS ((VOID_PARAM));
public void chg_caseless LESSPARAMS ((VOID_PARAM));
public int search LESSPARAMS ((int search_type, char *pattern, int n));
public void prep_hilite LESSPARAMS ((POSITION spos, POSITION epos, int maxlines));
public void set_filter_pattern LESSPARAMS ((char *pattern, int search_type));
public int is_filtering LESSPARAMS ((VOID_PARAM));
public RETSIGTYPE winch LESSPARAMS ((int type));
public void init_signals LESSPARAMS ((int on));
public void psignals LESSPARAMS ((VOID_PARAM));
public void cleantags LESSPARAMS ((VOID_PARAM));
public int gettagtype LESSPARAMS ((VOID_PARAM));
public void findtag LESSPARAMS ((char *tag));
public POSITION tagsearch LESSPARAMS ((VOID_PARAM));
public char * nexttag LESSPARAMS ((int n));
public char * prevtag LESSPARAMS ((int n));
public int ntags LESSPARAMS ((VOID_PARAM));
public int curr_tag LESSPARAMS ((VOID_PARAM));
public int edit_tagfile LESSPARAMS ((VOID_PARAM));
public void open_getchr LESSPARAMS ((VOID_PARAM));
public void close_getchr LESSPARAMS ((VOID_PARAM));
public int default_wheel_lines LESSPARAMS ((VOID_PARAM));
public int getchr LESSPARAMS ((VOID_PARAM));
