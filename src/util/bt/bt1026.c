
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BtContext.h"
#include "bt1026.h"

char at_urc_kw[32] = "";       // AT命令期望关键字 +kw
char at_urc_values[1024] = ""; // AT命令响应值

BtStat bt_stat = {
    0,
    0,
    0,
    0,
    0,
    0,
};

char bt_vers[64];
char bt_addr[12 + 1];           // 传统蓝牙地址
char bt_leaddr[12 + 1];         // 低功耗蓝牙地址
char bt_name[31 + 5 + 1];       // 传统蓝牙名称
char bt_lename[25 + 3 + 5 + 1]; // 低功耗蓝牙名称
PairedDevice bt_plist[9];       // 已配对的传统蓝牙 // 注: 蓝牙mac
int bt_plist_cnt = 0;

int data_count = 0;

static unsigned char checksumBCC(uint8_t *data, int length);
extern int on_ble_data(int cmd, char *data, int length);

static int default_callback(char *event, char *data, int len)
{
    char *p = NULL;

    if (strncmp(event, "GATTDATA", strlen("GATTDATA")) == 0)
    {
        p = strchr(data, ',');
        if (p)
        {
            //			if (strncmp(p + 1, "abcdefghijklmnopq", strlen("abcdefghijklmnopq")) == 0)
            data_count++;
        }
    }
    printf("%s: event name: %s, data_count: %d\n", __func__, event, data_count);

    printf("%s: data: %s\n", __func__, data);
    printf("%s: len: %d\n", __func__, len);

    return 0;
}


struct btEvent btEvents[BT_EVENTS_NUM] =
    {
        {"SCAN", default_callback},
        {"PAIRED", default_callback},
        {"HFPSTAT", default_callback},
        {"HFPDEV", default_callback},
        {"HFPCID", default_callback},
        {"HFPCIE", default_callback},
        {"HFPAUDIO", default_callback},
        {"HFPSIG", default_callback},
        {"HFPROAM", default_callback},
        {"HFPBATT", default_callback},
        {"HFPNET", default_callback},
        {"HFPMANU", default_callback},
        {"HFPNUM", default_callback},
        {"HFPIBR", default_callback},
        {"A2DPSTAT", default_callback},
        {"A2DPDEV", default_callback},
        {"AVRCPSTAT", default_callback},
        {"PLAYSTAT", default_callback},
        {"TRACKSTAT", default_callback},
        {"TRACKINFO", default_callback},
        {"PBSTAT", default_callback},
        {"PBCNT", default_callback},
        {"PBDATA", default_callback},
        {"SPPSTAT", default_callback},
        {"SPPDATA", default_callback},
        {"GATTSTAT", default_callback},
        {"GATTDATA", on_gattdata},
        {"HIDSTAT", default_callback},
        {"DEVSTAT", default_callback},
};

int isEvent(char *str, int len)
{
    int i;

    for (i = 0; i < sizeof(btEvents) / sizeof(btEvents[0]); i++)
    {
        // fprintf(stdout, "%s : %s\n", str + 1, btEvents[i].name);
        if (strncmp(str + 1, btEvents[i].name, strlen(btEvents[i].name)) == 0)
            return 1;
    }

    return 0;
}

int BtGetVersion(char *ver, int length)
{
    int ret = 0;

    assert(ver && length > 0);

    ret = BTCONTEXT->sendAt("AT+VER", "+VER", ver, length, strlen("AT+VER"), 100);

    return ret;
}

int BtGetBaud(char *baud, int length)
{
    int ret = 0;

    assert(baud && length > 0);

    ret = BTCONTEXT->sendAt("AT+BAUD", "+BAUD", baud, length, strlen("AT+BAUD"), 100);

    return ret;
}

int BtGetName(char *name, int length)
{
    int ret = 0;

    assert(name && length > 0);

    ret = BTCONTEXT->sendAt("AT+NAME", "+NAME", name, length, strlen("AT+NAME"), 100);

    return ret;
}

int BtSetName(char *name, int length)
{
    int ret = 0;
    char atbuf[128] = {0};

    assert(name && length > 0);

    snprintf(atbuf, 128, "AT+NAME=%s", name);
    ret = BTCONTEXT->sendAt(atbuf, "+NAME", NULL, 0, strlen(atbuf), 100);

    return ret;
}

// 透传数据回调(不需要用户注册)
#define BT_RECV_BUF_LEN        (1024 * 1024)
static char recv_buf[BT_RECV_BUF_LEN] = {0};
static int recv_cmd = 0;
static int recv_len = 0;
static int recv_need = 0;
int gattdata_process(char *buf, int len, void *priv)
{
#define HEAD_FLAG 0xFF
    int i;
    char *p = NULL;
    static unsigned char crc = 0;
#define DEBUG
#ifdef DEBUG
    fprintf(stdout, "len: %d,%s\n", len,buf);
    for (i = 0; i < len; i++)
    {
        fprintf(stdout, "%02x ", buf[i]);
        if ((i + 1) % 16 == 0)
            fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
#endif
    logi("%s:%d",__func__,__LINE__);
    p = buf;
    if (recv_need == 0)
    {
        if (p[0] != HEAD_FLAG && len < 6)
        {
            printf("%s head flag error p[0] = %2x, len: %d\n", __func__, p[0], len);
            return -1;
        }
        else
        {
            recv_need = (p[1] | p[2] << 8) + 6;
            recv_cmd = p[3];
            crc = p[5];
            fprintf(stdout, "recv_need: %d, p[1]: %02x, p[2]: %02x\n", recv_need, p[1], p[2]);
        }
    }

    if (recv_len + len > BT_RECV_BUF_LEN)
    {
    	fprintf(stderr, "data too large, recv_len: %d, len: %d, max: %d\n", recv_len, len, 1024 * 2);
    	recv_need = 0;
    	recv_len = 0;
    	return -1;
    }

    memcpy(recv_buf + recv_len, buf, len);
    recv_len += len;
    logi("recv_len = %d, recv_need = %d\n",recv_len,recv_need);
    if (recv_len == recv_need)
    {
        if (crc == checksumBCC((unsigned char *)recv_buf + 6, recv_len - 6))
        {
            logi("checksum success!!!\n");
            on_ble_data(recv_cmd, recv_buf + 6, recv_len - 6);
        }
        else
        {
            loge("checksum error, recv_len = %d,%02x\n",recv_len,p[5]);
        }

#ifdef DEBUG

        for (i = 0; i < recv_len; i++)
        {
            fprintf(stdout, "%02x ", recv_buf[i]);
            if ((i + 1) % 16 == 0)
                fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
#endif

        memset(recv_buf, 0, sizeof(recv_buf));
        recv_len = 0;
        recv_need = 0;
    }

    return 0;
}

int on_gattdata(char *urc, char *buf, int len)
{
    //    logw("on_gattdata %s %d %s", urc, len, buf);
    char *p = NULL;
    p = strchr(buf, ',');
    if (p)
    {
        *p = '\0';
        len -= strlen(buf) + 1;
        //		fprintf(stderr, "%s: buf: %s, len: %d\n", __func__, p + 1, len);
        gattdata_process(p + 1, len, NULL);
    }
    else
    {
        fprintf(stderr, "%s: buf: %s, len: %d\n", __func__, buf, len);
    }

    return 0;
}

// 统计逗号数量
int fieldCount(char *input)
{
    int count = 0;

    char *token = strtok(input, ",");
    while (token != NULL)
    {
        // if (fields) {
        //     fields[count] = token;
        // }
        count++;
        token = strtok(NULL, ",");
    }
    return count;
}

// 解析逗号分隔的整数字符串
int parseInts(char *input, int arr[], int size)
{
    int count = 0;

    char *token = strtok(input, ",");
    while (token != NULL)
    {
        int num = atoi(token);
        arr[count++] = num;
        if (count >= size)
            break;
        token = strtok(NULL, ",");
    }
    return count;
}

// 解析 AT+PLIST 记录
int parsePlist(char *fields, int *pSn, char **pMac, char **pOptName)
{
    *pSn = atoi(fields);

    char *comma1 = strchr(fields, ',');
    if (!comma1)
        return -1;
    *comma1 = 0;
    *pMac = comma1 + 1;

    char *comma2 = strchr(comma1 + 1, ',');
    // if (!comma2) return -1;
    if (comma2)
    {
        *comma2 = 0;
        *pOptName = comma2 + 1;
    }
    else
    {
        *pOptName = NULL;
    }
    return 0;
}

void cb_plist(const char *urc, const char *value)
{
    // +PLIST=sn,mac{,utf8name}
    if (strcmp(urc, "+PLIST") != 0)
    {
        assert(urc == "+PLIST");
        return;
    }
    // pln("cb_plist %s", value);
    if (strcmp(value, "E") == 0)
    {
        // pln("bt_plist END cnt=%d", bt_plist_cnt);
        for (int i = 1; i <= bt_plist_cnt; i++)
        {
            logi("bt_plist [%d] addr=%s, name=%s", i, bt_plist[i].addr, bt_plist[i].utf8Name);
        }
    }
    else
    {
        int idx = 0;
        char *pMac, *pOptName;
        char *dup_value = strdup(value);
        if (!dup_value)
            return;
        if (parsePlist(dup_value, &idx, &pMac, &pOptName) == 0)
        {
            assert(idx <= 8);
            strncpy(bt_plist[idx].addr, pMac, sizeof(bt_plist[0].addr));
            if (pOptName)
            {
                strncpy(bt_plist[idx].utf8Name, pOptName, sizeof(bt_plist[0].utf8Name));
            }
            else
            {
                bt_plist[idx].utf8Name[0] = 0;
            }
            bt_plist_cnt = idx;
        }
        free(dup_value);
    }
}

void cb_xstat(const char *urc, const char *value)
{
    const char *TabGeneralSt[] = {
        "ST_UNSUPPORTED", // 0,
        "ST_STANDBY",     // 1,
        "ST_CONNECTING",  // 2,
        "ST_CONNECTED",   // 3,
        "ST_STREAMING",   // 4,
        //
        "ST_OUTGOING_CALL", // 4,
        "ST_INCOMING_CALL", // 5,
        "ST_ACTIVE_CALL",   // 6,
    };
    int ival = atoi(value);
    logi("ui todo: on %s=%X", urc, ival);

    if (strcmp(urc, "+DEVSTAT") == 0)
    {
        int chg = bt_stat.devstat ^ ival;
        bt_stat.devstat = ival;
        if (chg & DEV_STAT_MASK_POWER_OFF)
        {
            logi("POWER_OFF => %d", ival & DEV_STAT_MASK_POWER_OFF);
        }
        if (chg & DEV_STAT_MASK_BR_EDR_DISCOVERABLE)
        {
            logi("BR_EDR_DISCOVERABLE => %d", ival & DEV_STAT_MASK_BR_EDR_DISCOVERABLE);
        }
        if (chg & DEV_STAT_MASK_BLE_ADVERTISING)
        {
            logi("BLE_ADVERTISING => %d", ival & DEV_STAT_MASK_BLE_ADVERTISING);
        }
    }
    else
    {
        assert(ival >= 0 && ival <= 6);
        if (strcmp(urc, "+HFPSTAT") == 0)
        {
            bt_stat.hfpstat = ival;
            if (ival >= 4)
                ival += 1;
            logi("HFPSTAT => %s", TabGeneralSt[ival]);
        }
        else if (strcmp(urc, "+A2DPSTAT") == 0)
        {
            bt_stat.a2dpstat = ival;
            logi("A2DPSTAT => %s", TabGeneralSt[ival]);
        }
        else if (strcmp(urc, "+AVRCPSTAT") == 0)
        {
            bt_stat.avrcpstat = ival;
            logi("AVRCPSTAT => %s", TabGeneralSt[ival]);
        }
        else if (strcmp(urc, "+SPPSTAT") == 0)
        {
            bt_stat.sppstat = ival;
            logi("SPPSTAT => %s", TabGeneralSt[ival]);
        }
        else if (strcmp(urc, "+GATTSTAT") == 0)
        {
            bt_stat.gattstat = ival;
            logi("GATTSTAT => %s", TabGeneralSt[ival]);
        }
    }
}
void cb_trackinfo(const char *urc, const char *value)
{
    // +TRACKINFO=Not Provided
    logi("cb_trackinfo %s", value);
}
void cb_trackstat(const char *urc, const char *value)
{
    //  +TRACKSTAT=1,570212,570212 或 +TRACKSTAT=1,0,0
    logi("cb_trackstat %s", value);
}
void cb_playstat(const char *urc, const char *value)
{
    const char *PlayStatDef[] = {
        "Stopped",
        "Playing",
        "Paused",
        "Fast Forwarding",
        "Fast Rewinding",
    };
    int ival = atoi(value);
    if (ival > 4)
        return;
    logi("cb_playstat %s == %s", value, PlayStatDef[ival]);
}
void cb_paired(const char *urc, const char *value)
{
    logi("cb_paired %s ", value);
}

// 用户注册的回调
UrcCbItem urc_callbacks[16] = {
    {"+PLIST", cb_plist},   // 已配对信息
    {"+DEVSTAT", cb_xstat}, // 各种连接状态变化
    {"+HFPSTAT", cb_xstat},
    {"+A2DPSTAT", cb_xstat},
    {"+AVRCPSTAT", cb_xstat},
    {"+SPPSTAT", cb_xstat},
    {"+GATTSTAT", cb_xstat},
    // {"+HIDSTAT", cb_xstat},
    // {"+PBSTAT", cb_xstat},
    {"+TRACKINFO", cb_trackinfo}, // 当前歌曲名称
    {"+TRACKSTAT", cb_trackstat}, // 播放进度
    {"+PLAYSTAT", cb_playstat},   // 播放暂停等状态变化
    {"+PAIRED", cb_paired},       // 注意：从 +GATTDEV 得到的地址是变化的，不能作为设备唯一标识。
    //
    {NULL, NULL}};

int bt_init()
{
    // 蓝牙初始化:
    // AT握手，切换到指定波特率;
    // 数传设置为指令模式；
    // 读取固件版本号、设备状态 等信息。
    // 设置蓝牙名称；
    // 读取配对列表；

    // todo： 设置配对模式和密码

    char version[264] = {0};
    char name[128] = {0};

    pln("bt_init ...");
    BtContext *ctx = BtContext::getInstance();

    if (!bt_at())
        return -1;

    int res = 0;
    res = ctx->sendAt("AT+TPMODE", "+TPMODE", NULL, 0, strlen("AT+TPMODE"), 100);
    if (res != 0)
    {
        return -1;
    }
    res = ctx->sendAt("AT+TPMODE=0", NULL, NULL, 0, strlen("AT+TPMODE=0"), 100);
    if (res != 0)
    {
        return -1;
    }

    bt_get_ver();
    bt_set_name("GolfCar");

    BtGetName(name, 128);
    fprintf(stdout, "name: %s\n", name);

    bt_read_st();
    bt_read_paired();

    BtGetVersion(version, 260);
    fprintf(stdout, "get version: %s\n", version);

    // test
    // bt_reboot();

    // // test
    // Thread::sleep(10 * 1000);
    // sprintf(at_urc_values, "Hello phone");
    // bt_tx(at_urc_values);

    return 0;
}

// 注册 URC 回调函数
// 例如 AT+PLIST - 读取配对记录
// +PLIST=1,F487C5984689,HUAWEI Mate 40 Pro
// +PLIST=2,A06974532BCB,002022050420061301045
// +PLIST=3,288FF6578E1C,wenkun的iPhone
// +PLIST=E
// OK
int bt_set_urc_cb()
{
    return 0;
}

bool bt_at()
{
    BtContext *ctx = BtContext::getInstance();
    int i = 0;
    // 尝试不同波特率
    for (i = 0; i < 3; i++)
    {
        ctx->send("\r\n\r\n", 4);
        Thread::sleep(100);
        // int res = ctx->sendAt("AT");
        //        if (res == AtStOk)
        break;
        //        pln("bt_init AT fail %d", i);
        ctx->openUart(BT_DEV, ctx->nextBaud());
    }
    if (i >= 3)
        return false;

    // // 切换波特率
    // if (ctx->mBaudRate != B115200) {
    //     int res = ctx->sendAt("AT+BAUD=115200");
    //     if (res != AtStOk)
    //         return false;
    //     bt_at()
    // }

    return true;
}

bool bt_reboot()
{
    // AT+REBOOT
    return 0 == BTCONTEXT->sendAt("AT+REBOOT", "+REBOOT", NULL, 0, strlen("AT+REBOOT"), 100);
}

bool bt_spk_vol()
{
    // AT+SPKVOL
    return true;
}

bool bt_get_ver()
{
    // AT+VER
    return 0 == BTCONTEXT->sendAt("AT+VER", "+VER", bt_vers, sizeof(bt_vers), strlen("AT+VER"), 100);
}

bool bt_take_photo(void)
{
    int ret = 0;
    char buf[128] = {0};

    ret = snprintf(buf, 128, "AT+HIDCMD=");
    buf[ret + 1] = 0xE9;
    buf[ret + 2] = 0x0D;
    buf[ret + 3] = 0x0A;
    buf[ret + 4] = '\0';

    return BTCONTEXT->send(buf, ret + 4);
}

bool bt_get_mac()
{
    // AT+ADDR
    // AT+LEADDR
    int ret = 0;
    ret = BTCONTEXT->sendAt("AT+ADDR", "+ADDR", bt_addr, sizeof(bt_addr), strlen("AT+ADDR"), 200);
    fprintf(stdout, "%s: bt_addr: %s, ret: %d\n", __func__, bt_addr, ret);

    if (ret == 0)
        return true;
    else
        return false;
}

// 设置蓝牙名称
bool bt_set_name(const char *newname)
{
    int with_postfix = 0;
    char want_full_name[sizeof(bt_name)] = {0};
    char cmd_buf[64];
    BtContext *ctx = BtContext::getInstance();

    fprintf(stdout, "%s get_mac now\n", __func__);
    if (!bt_get_mac())
    {
        loge("get mac fail");
        return false;
    }

    // AT+NAME - 读/写BR/EDR蓝牙名称
    // sprintf(want_full_name, "%s-%s", newname, bt_addr + 8);
    sprintf(want_full_name, "%s-%s", newname, bt_addr + 8);

    int res = ctx->sendAt("AT+NAME", "+NAME", bt_name, sizeof(bt_name), strlen("AT+NAME"), 100);
    if (res != 0)
    {
        return false;
    }
    if (strcmp(want_full_name, bt_name) != 0)
    {
        logw("want %s but %s", want_full_name, bt_name);
        sprintf(cmd_buf, "AT+NAME=%s,%d", want_full_name, !!with_postfix);
        res = ctx->sendAt(cmd_buf, "+NAME", NULL, 0, strlen(cmd_buf), 100);
        if (res != 0)
        {
            return false;
        }
    }

    // AT+LENAME - 读/写BLE蓝牙名称
    // sprintf(want_full_name, "%s-LE-%s", newname, bt_addr + 8);
    sprintf(want_full_name, "%s-LE-%s", newname, bt_addr + 8);

    res = ctx->sendAt("AT+LENAME", "+LENAME", bt_lename, sizeof(bt_lename), strlen("AT+LENAME"), 100);
    if (res != 0)
    {
        return false;
    }
    if (strcmp(want_full_name, bt_lename) != 0)
    {
        sprintf(cmd_buf, "AT+LENAME=%s,%d", want_full_name, !!with_postfix);
        res = ctx->sendAt(cmd_buf, "+LENAME", NULL, 0, strlen(cmd_buf), 100);
        if (res != 0)
        {
            return false;
        }
    }

    return true;
}

// 读所有状态
bool bt_read_st()
{
    // AT+STAT - 读所有状态 *
    char buf[32];
    int res = BTCONTEXT->sendAt("AT+STAT", "+STAT", buf, sizeof(buf), strlen("AT+STAT"), 100);
    if (res != 0)
    {
        return false;
    }
    int arr[8] = {0};
    int cnt = parseInts(buf, arr, 8);
    assert(cnt == 8);
    int i = 0;
    bt_stat.devstat = arr[i++];   // bits
    bt_stat.sppstat = arr[i++];   // 123
    bt_stat.gattstat = arr[i++];  // 123
    bt_stat.hfpstat = arr[i++];   // 123+456 // Outgoing call, Incoming call, Active call
    bt_stat.a2dpstat = arr[i++];  // 123+4 // streaming
    bt_stat.avrcpstat = arr[i++]; // 123
                                  // bt_stat.hidstat = arr[i++];// 123
                                  // bt_stat.pbstat = arr[i++];// 123+4

    pln("==============================================================================");
    pln("ST_UNSUPPORTED = 0, ST_STANDBY = 1, ST_CONNECTING = 2, ST_CONNECTED = 3, ST_STREAMING = 4");
    pln("==============================================================================");
    pln("bt_stat.devstat=%02x,sppstat=%d,gattstat=%d,hfpstat=%d,a2dpstat=%d,avrcpstat=%d",
        bt_stat.devstat, bt_stat.sppstat, bt_stat.gattstat, bt_stat.hfpstat, bt_stat.a2dpstat, bt_stat.avrcpstat);

    return true;
}

// 读取配对记录
bool bt_read_paired()
{
    // AT+PLIST - 读取/清除配对记录
    // +PLIST=1,F487C5984689,HUAWEI Mate 40 Pro
    // +PLIST=2,A06974532BCB,002022050420061301045
    // +PLIST=3,288FF6578E1C,wenkun的iPhone
    // +PLIST=E
    // OK
    return 0 == BTCONTEXT->sendAt("AT+PLIST", "+PLIST", NULL, 0, strlen("AT+PLIST"), 100);
}

// 设置配对模式 & 配对密码
bool bt_set_pair_mode(int mode, const char *pin)
{
    // AT+SSP - 读/写BR/EDR配对模式 *
    // AT+PIN - 读/写配对密码
    int res = BTCONTEXT->sendAt("AT+SSP=2", "+SSP", NULL, 0, strlen("AT+SSP=2"), 100);
    int res2 = BTCONTEXT->sendAt("AT+PIN=2023", "+PIN", NULL, 0, strlen("AT+PIN=2023"), 100);

    return res == 0 && res2 == 0;
}

// 蓝牙开关
bool bt_sw(int on)
{
    // AT+BTEN - 开关蓝牙
    char cmd_buf[16];
    sprintf(cmd_buf, "AT+BTEN=%d", !!on);
    return 0 == BTCONTEXT->sendAt(cmd_buf, "+BTEN", NULL, 0, strlen(cmd_buf), 100);
}

// 数据透传.发送 (接收由回调实现)
int bt_tx(const char *json)
{
    // AT+GATTSEND - 通过GATT发数据

    // TODO - 分块发送，暂不考虑附加帧头
    int json_len = strlen(json);
    int blk_cnt = (json_len + 240) / 241;
    // if (json_len > 241)
    {
        pln("bt_tx json_len %d chuncks %d", json_len, blk_cnt);
    }
    for (int i = 0; i < blk_cnt; i++)
    {
        int blk_len = 241;
        if (i == blk_cnt - 1)
            blk_len = json_len - 241 * i;

        if (AtStOk != BTCONTEXT->sendBlk(json + 241 * i, blk_len, 100))
        {
            loge("sendBlk fail %d", i);
            return -1;
        }
        else
        {
            loge("sendBlk ok %d", i);
        }
    }

    return 0;
}

bool bt_set_autoconn(int cnt)
{
    // AT+AUTOCONN - 设置蓝牙上电重连次数
    char cmd_buf[32];
    sprintf(cmd_buf, "AT+AUTOCONN=%d", cnt);
    return 0 == BTCONTEXT->sendAt(cmd_buf, "+AUTOCONN", NULL, 0, strlen(cmd_buf), 100);
}

static unsigned char checksumBCC(uint8_t *data, int length)
{
    uint8_t checksum = 0;
    for (int i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }
    return checksum;
}

int bt_send_raw(const char *data, int len, int timeout)
{
    return BTCONTEXT->sendBlk(data, len, timeout);
}

int ble_send_data(int cmd, uint8_t *pdata, short datalen, int timeout)
{
#define PACKET_SIZE 230
    int packets = 0;
    int left_data = 0;
    int data_index = 0;
    unsigned char buf[512] = {0};
    unsigned char crc = 0;

    // for (int i = 0; i < datalen; i++)
    // {
    //         fprintf(stdout, "%02x ", pdata[i]);
    //         if ((i + 1) % 16 == 0)
    //                 fprintf(stdout, "\n");
    // }
    // fprintf(stdout, "\n");

    crc = checksumBCC(pdata, datalen);
    packets = (6 + datalen + PACKET_SIZE - 1) / PACKET_SIZE;
    if (packets == 1)
    {
        left_data = datalen;
    }
    else
    {
        left_data = (6 + datalen) % PACKET_SIZE;
    }
    printf("packets = %d,left = %d\n", packets, left_data);

    for (int i = 0; i < packets; i++)
    {
        printf("cur packets = %d\n", i + 1);
        int buf_idx = 0;
        int blk_len = PACKET_SIZE;
        if (i == 0)
        {
            buf[0] = 0xff;
            buf[1] = 0xFF & datalen;
            buf[2] = (datalen >> 8) & 0xFF;
            buf[3] = cmd;
            buf[4] = packets;
            buf[5] = crc;
            buf_idx = 6;
            blk_len -= 6;
        }

        if (i == packets - 1)
        {
            blk_len = left_data;
        }

        // printf("packets = %d,left === %d\n",packets,left_data);
        memcpy(buf + buf_idx, pdata + data_index, blk_len);
        data_index += blk_len;

        int res = bt_send_raw((const char *)buf, buf_idx + blk_len, 100);
        if (res != AtStOk)
        {
            loge("send failed bt_send_raw = %d\n", res);
            return -1;
        }
        //usleep(100 * 1);
        // printf("\n");
        // for(int j = 0; j < buf_idx + blk_len; j++)
        // {
        //     printf("%02x ",buf[j]);
        //     if((j+1) % 16 ==0)
        //     {
        //             printf("\n");
        //     }
        // }
        // printf("\n");
    }

    return datalen;
}

int ble_send_test(int cmd, uint8_t *pdata, short datalen)
{
#define PACKET_SIZE 230
    int packets = 0;
    int left = 0;
    int index = 0;
    int ret = 0;

    packets = datalen / PACKET_SIZE;
    left = datalen % PACKET_SIZE;
    printf("packets = %d,left = %d\n", packets, left);

    for (int i = 0; i < packets; i++)
    {
        ret = bt_send_raw((const char *)pdata + index, PACKET_SIZE, 0);
        if (ret != AtStOk)
        {
            loge("send failed bt_send_raw = %d\n", ret);
            return -1;
        }
        index += PACKET_SIZE;
        usleep(100 * 1);
    }

    if (left)
    {
        ret = bt_send_raw((const char *)pdata + index, left, 0);
        if (ret != AtStOk)
        {
            loge("send failed bt_send_raw = %d, left: %d\n", ret, left);
            return -1;
        }
        index += left;
        usleep(100 * 1);
    }

    return datalen;
}
