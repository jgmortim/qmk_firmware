#pragma once

// windows_functions.c
void windows_run(const char *command);
void send_alt_code(const char code[]);

// linux_functions.c
void linux_run(const char *command);
void send_unicode(const char *code);
