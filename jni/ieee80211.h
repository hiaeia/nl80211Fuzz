#ifndef LINUX_IEEE80211_H
#define LINUX_IEEE80211_H

#define WLAN_MAX_KEY_LEN        32
#define WLAN_PMKID_LEN          16
#define NL80211_MAX_SUPP_RATES          32
#define NL80211_MAX_SUPP_HT_RATES       77
#define NL80211_MAX_SUPP_REG_RULES      32
#define NL80211_TKIP_DATA_OFFSET_ENCR_KEY   0
#define NL80211_TKIP_DATA_OFFSET_TX_MIC_KEY 16
#define NL80211_TKIP_DATA_OFFSET_RX_MIC_KEY 24
#define NL80211_HT_CAPABILITY_LEN       26
#define NL80211_VHT_CAPABILITY_LEN      12

#define NL80211_MAX_NR_CIPHER_SUITES        5
#define NL80211_MAX_NR_AKM_SUITES       2

#define NL80211_MIN_REMAIN_ON_CHANNEL_TIME  10

/* default RSSI threshold for scan results if none specified. */
#define NL80211_SCAN_RSSI_THOLD_OFF     -300

#define NL80211_CQM_TXE_MAX_INTVL       1800
#define IEEE80211_MAX_FRAG_THRESHOLD    2352
#define IEEE80211_MAX_RTS_THRESHOLD 2353
#define IEEE80211_MAX_AID       2007
#define IEEE80211_MAX_TIM_LEN       251
/* Maximum size for the MA-UNITDATA primitive, 802.11 standard section
   6.2.1.1.2.

   802.11e clarifies the figure in section 7.1.2. The frame body is
   up to 2304 octets long (maximum MSDU size) plus any crypt overhead. */
#define IEEE80211_MAX_DATA_LEN      2304
/* 802.11ad extends maximum MSDU size for DMG (freq > 40Ghz) networks
 * to 7920 bytes, see 8.2.3 General frame format
 */
#define IEEE80211_MAX_DATA_LEN_DMG  7920
/* 30 byte 4 addr hdr, 2 byte QoS, 2304 byte MSDU, 12 byte crypt, 4 byte FCS */
#define IEEE80211_MAX_FRAME_LEN     2352

#define IEEE80211_MAX_SSID_LEN      32

#define IEEE80211_MAX_MESH_ID_LEN   32

#define IEEE80211_NUM_TIDS      16

#define IEEE80211_QOS_CTL_LEN       2
/* 1d tag mask */
#define IEEE80211_QOS_CTL_TAG1D_MASK        0x0007
/* TID mask */
#define IEEE80211_QOS_CTL_TID_MASK      0x000f
/* EOSP */
#define IEEE80211_QOS_CTL_EOSP          0x0010
/* ACK policy */
#define IEEE80211_QOS_CTL_ACK_POLICY_NORMAL 0x0000
#define IEEE80211_QOS_CTL_ACK_POLICY_NOACK  0x0020
#define IEEE80211_QOS_CTL_ACK_POLICY_NO_EXPL    0x0040
#define IEEE80211_QOS_CTL_ACK_POLICY_BLOCKACK   0x0060
#define IEEE80211_QOS_CTL_ACK_POLICY_MASK   0x0060
/* A-MSDU 802.11n */
#define IEEE80211_QOS_CTL_A_MSDU_PRESENT    0x0080
/* Mesh Control 802.11s */
#define IEEE80211_QOS_CTL_MESH_CONTROL_PRESENT  0x0100

/* Mesh Power Save Level */
#define IEEE80211_QOS_CTL_MESH_PS_LEVEL     0x0200
/* Mesh Receiver Service Period Initiated */
#define IEEE80211_QOS_CTL_RSPI          0x0400

/* U-APSD queue for WMM IEs sent by AP */
#define IEEE80211_WMM_IE_AP_QOSINFO_UAPSD   (1<<7)
#define IEEE80211_WMM_IE_AP_QOSINFO_PARAM_SET_CNT_MASK  0x0f

/* U-APSD queues for WMM IEs sent by STA */
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_VO  (1<<0)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_VI  (1<<1)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_BK  (1<<2)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_BE  (1<<3)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_MASK    0x0f

/* U-APSD max SP length for WMM IEs sent by STA */
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_ALL 0x00
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_2   0x01
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_4   0x02
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_6   0x03
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_MASK    0x03
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_SHIFT   5

#endif
