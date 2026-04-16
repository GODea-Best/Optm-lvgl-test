#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

/* --- Screen transition animation duration (ms) --- */
#define CFG_SLIDE_MS                500

/* --- Service Alert screen --- */
#define CFG_SERVICE_ALERT_DATE      "06/28/1985"
#define CFG_SERVICE_ALERT_DAYS      "28 Days Remain"

/* --- Service Warning screen --- */
#define CFG_SERVICE_WARNING_DATE    "11/16/2026"
#define CFG_SERVICE_WARNING_DAYS    "4 Days Remain"

/* --- Violation Warning screen --- */
#define CFG_VIOLATION_DATE          "10/28/2026"
#define CFG_VIOLATION_DAYS          "3 Days Remain"

/* --- Free Restart screen (MM:SS.hh) --- */
#define CFG_FREE_RESTART_TIME       "01:29.56"

/* --- Diagnostic screen --- */
#define CFG_DIAGNOSTIC_VOLUME       "75%"
#define CFG_DIAGNOSTIC_DURATION     "90%"
#define CFG_DIAGNOSTIC_VOLUME_PCT   75
#define CFG_DIAGNOSTIC_DURATION_PCT 90

/* --- Initial Test Pass screen --- */
#define CFG_BRAC_VALUE              "0.00%"

/* --- USA Logo 3D screen --- */
/* Delay (ms) before the sheen wipe starts after the screen has loaded.
   Should be >= CFG_SLIDE_MS so the wipe doesn't begin mid-transition. */
#define CFG_USA_3D_SHEEN_DELAY_MS   800

/* --- Blow Prompt screen --- */
#define CFG_BLOW_PROMPT_MSG         "Breathe steadily\ninto the device"

/* --- Violation Lockout screen --- */
#define CFG_LOCKOUT_DESC            "Contact service\nto unlock"

/* --- Retest Alert screen (MM:SS.hh) --- */
#define CFG_RETEST_TIME             "04:14.20"

#endif /* DEVICE_CONFIG_H */
