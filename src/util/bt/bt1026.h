#ifndef BT1026_H
#define BT1026_H

#define OK		"OK"
#define ERROR	"ERROR"
#define CRLF_OK "\r\nOK\r\n"
#define CRLF_ERROR "\r\nERROR\r\n"
#define CRLF "\r\n"
#define CR '\r'
#define LF '\n'

enum AtStat {
    AtStIdle = 0,
    AtStWaiting = 1,
    AtStOk = 2,
    AtStErr = 3,
    AtStTimeout = 4,
    AtStFalt = 5,
};

// Command AT+STAT
// Response +STAT=Param1, Param2, Param3…
// 例如 +STAT=5,1,1,3,4,3,0,3
// Param1 DEVSTAT
// Param2 SPPSTAT
// Param3 GATTSTAT
// Param4 HFPSTAT
// Param5 A2DPSTAT
// Param6 AVRCPSTAT
// Param7 HIDSTAT
// Param8 PBSTAT
typedef struct {
    int devstat;   // bits
    int sppstat;   // 123
    int gattstat;  // 123
    int hfpstat;   // 123+456 // Outgoing call, Incoming call, Active call
    int a2dpstat;  // 123+4 // streaming
    int avrcpstat; // 123
    // int hidstat;// 123
    // int pbstat;// 123+4
} BtStat;

// Command AT+DEVSTAT
// Response +DEVSTAT=Param
// BIT[0] 0: Power Off; 1: Power On
// BIT[1] 0: BR/EDR Not Discoverable; 1: BR/EDR Discoverable
// BIT[2] 0: BLE Not Advertising; 1: BLE Advertising
// BIT[3] 0: BR/EDR Not Scanning; 1: BR/EDR Scanning
// BIT[4] 0: BLE Not Scanning; 1: BLE Scanning
#define DEV_STAT_MASK_POWER_OFF (1 << 0)
#define DEV_STAT_MASK_BR_EDR_DISCOVERABLE (1 << 1)
#define DEV_STAT_MASK_BLE_ADVERTISING (1 << 2)
// DEV_STAT_MASK_BR_EDR_SCANNING = 1 << 3,
// DEV_STAT_MASK_BLE_SCANNING = 1 << 4,

enum GeneralSt {
    ST_UNSUPPORTED = 0,
    ST_STANDBY = 1,
    ST_CONNECTING = 2,
    ST_CONNECTED = 3,
    ST_STREAMING = 4,
    // not use 4,5,6
    ST_OUTGOING_CALL = 4,
    ST_INCOMING_CALL = 5,
    ST_ACTIVE_CALL = 6,
};

typedef struct {
    char addr[13];
    char utf8Name[128];
} PairedDevice;

// Command AT+VER
// Response +VER=Param1,Param2,Param3
// Param1 模块类型
// Param2 固件版本
// Param3 固件编译时间

extern char at_urc_kw[];
extern char at_urc_values[];

extern BtStat bt_stat;
extern char bt_vers[];
extern char bt_addr[];
extern char bt_leaddr[];
extern char bt_name[];
extern char bt_lename[];
extern PairedDevice bt_plist[];
extern int bt_plist_cnt;

int bt_init();
int bt_set_urc_cb();
bool bt_at();
bool bt_reboot();

bool bt_get_ver();
bool bt_get_mac();
int parseInts(char *input, int arr[], int size);
bool bt_read_st();
bool bt_read_paired();

bool bt_sw(int on);
bool bt_set_name(const char *newname);
bool bt_set_pair_mode(int mode, const char *pin);
bool bt_set_autoconn(int cnt);
int bt_tx(const char *json);

typedef void (*UrcCallback)(const char *urc, const char *value);
typedef struct
{
    const char *urc;
    UrcCallback cb;
} UrcCbItem;

extern UrcCbItem urc_callbacks[];

int on_gattdata(const char *urc, char *buf, int len);
// void cb_plist(const char *urc, const char *value);

#define BT_EVENTS_NUM 28
struct btEvent {
	const char *name;
	int (*callback)(char *name, char *data, int len);
};

extern struct btEvent btEvents[];

int isEvent(char *str, int len);

#endif
