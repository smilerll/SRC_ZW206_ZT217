/*******************************************************************************
 ** File Name:     atc_def.h                                                   *
 ** Author:        Steven.Yao                                                  *
 ** DATE:          04/11/2002                                                  *
 ** Copyright:     2001 Spreadtrum, Incoporated. All Rights Reserved.          *
 ** Description:   This file defines the AT commands, including basic commands *
 **                a extend commands.                                          *
 *******************************************************************************
                                                                               *
 *******************************************************************************
 **                         Edit History                                       *
 **----------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                                *
 ** 04/11/2002     Steven.Yao       Create.                                    *
 ******************************************************************************/
#ifndef _ATC_DEF_H
#define _ATC_DEF_H

/* Define AT command index */

#define AT_CMD_AT       0
#define AT_CMD_SHARP_SHARP       1
#define AT_CMD_D       2
#define AT_CMD_Q       3
#define AT_CMD_V       4
#define AT_CMD_A       5
#define AT_CMD_H       6
#define AT_CMD_Z       7
#define AT_CMD_E       8
#define AT_CMD_and_C       9
#define AT_CMD_and_D      10
#define AT_CMD_and_F      11
#define AT_CMD_S7      12
#define AT_CMD_CPIN      13
#define AT_CMD_CCFC      14
#define AT_CMD_CCWA      15
#define AT_CMD_CREG      16
#define AT_CMD_VTS      17
#define AT_CMD_CHLD      18
#define AT_CMD_CLCC      19
#define AT_CMD_CPAS      20
#define AT_CMD_COPS      21
#define AT_CMD_SIND      22
#define AT_CMD_CSQ      23
#define AT_CMD_CIMI      24
#define AT_CMD_CGMI      25
#define AT_CMD_CGMM      26
#define AT_CMD_CGSN      27
#define AT_CMD_CGMR      28
#define AT_CMD_CMEE      29
#define AT_CMD_CSCS      30
#define AT_CMD_CPOF      31
#define AT_CMD_CPOL      32
#define AT_CMD_S0      33
#define AT_CMD_CGDCONT      34
#define AT_CMD_CGPCO      35
#define AT_CMD_CGQREQ      36
#define AT_CMD_CGQMIN      37
#define AT_CMD_CGATT      38
#define AT_CMD_CGACT      39
#define AT_CMD_CGCMOD      40
#define AT_CMD_CGDATA      41
#define AT_CMD_CGEREP      42
#define AT_CMD_CGREG      43
#define AT_CMD_CSMS      44
#define AT_CMD_CSDH      45
#define AT_CMD_CRES      46
#define AT_CMD_CPMS      47
#define AT_CMD_CMGF      48
#define AT_CMD_CMGR      49
#define AT_CMD_CMGL      50
#define AT_CMD_CMGD      51
#define AT_CMD_CMGS      52
#define AT_CMD_CMSS      53
#define AT_CMD_CSMP      54
#define AT_CMD_CMGC      55
#define AT_CMD_CMGW      56
#define AT_CMD_CSCA      57
#define AT_CMD_CSAS      58
#define AT_CMD_SMSC      59
#define AT_CMD_CSCB      60
#define AT_CMD_CNMI      61
#define AT_CMD_CPBR      62
#define AT_CMD_CPBF      63
#define AT_CMD_CPBW      64
#define AT_CMD_CPBP      65
#define AT_CMD_CPBN      66
#define AT_CMD_CNUM      67
#define AT_CMD_SSMP      68
#define AT_CMD_CPBS      69
#define AT_CMD_CCUG      70
#define AT_CMD_CAOC      71
#define AT_CMD_CCLK      72
#define AT_CMD_XX      73
#define AT_CMD_CCID      74
#define AT_CMD_VGR      75
#define AT_CMD_CMUT      76
#define AT_CMD_SSEA      77
#define AT_CMD_SBAND      78
#define AT_CMD_SDTMF      79
#define AT_CMD_CLCK      80
#define AT_CMD_CPWD      81
#define AT_CMD_NVGV      82
#define AT_CMD_NVPC      83
#define AT_CMD_xor_SSIT      84
#define AT_CMD_SSAM      85
#define AT_CMD_SSAP      86
#define AT_CMD_SADM      87
#define AT_CMD_SPLO      88
#define AT_CMD_CFUN      89
#define AT_CMD_CIND      90
#define AT_CMD_CCED      91
#define AT_CMD_STONE      92
#define AT_CMD_SSST      93
#define AT_CMD_SATT      94
#define AT_CMD_SAUTOATT      95
#define AT_CMD_SGPRSDATA      96
#define AT_CMD_SDMUT      97
#define AT_CMD_SUPS      98
#define AT_CMD_SSYS      99
#define AT_CMD_SCPFRE     100
#define AT_CMD_CGEQREQ     101
#define AT_CMD_CGEQMIN     102
#define AT_CMD_SCINF     103
#define AT_CMD_SFSMSM     104
#define AT_CMD_SPREF     105
#define AT_CMD_SYTIN     106
#define AT_CMD_SFTIN     107
#define AT_CMD_CMUX     108
#define AT_CMD_SIME     109
#define AT_CMD_SCMUX     110
#define AT_CMD_PSORGMOD     111
#define AT_CMD_SPENHA     112
#define AT_CMD_X     113
#define AT_CMD_SPAUDVOLTYPE     114
#define AT_CMD_SPDIAG     115
#define AT_CMD_SPBTTEST     116
#define AT_CMD_SPBTCTRL     117
#define AT_CMD_I1     118
#define AT_CMD_I2     119
#define AT_CMD_I3     120
#define AT_CMD_I4     121
#define AT_CMD_I5     122
#define AT_CMD_I6     123
#define AT_CMD_I7     124
#define AT_CMD_GMM     125
#define AT_CMD_FCLASS     126
#define AT_CMD_GCI     127
#define AT_CMD_SHARP_CLS     128
#define AT_CMD_SPDUMMYKPD     129
#define AT_CMD_SPDUMMYKPDRSP     130
#define AT_CMD_SPAUTOPOWER     131
#define AT_CMD_L2CAP     132
#define AT_CMD_SPSMSFULL     133
#define AT_CMD_CUSD     134
#define AT_CMD_SPFMMODE     135
#define AT_CMD_SPFMTUNE     136
#define AT_CMD_RWRFREG     137
#define AT_CMD_SPFMTEST     138
#define AT_CMD_SPUSIMTEST     139
#define AT_CMD_ETSRESET     140
#define AT_CMD_xor_SPDATAENCRYPT     141
#define AT_CMD_xor_SPSIMLOCKDATAWRITE     142
#define AT_CMD_xor_SPSIMLOCKIMEI     143
#define AT_CMD_xor_SPHUK     144
#define AT_CMD_xor_SPFACAUTHPUBKEY     145
#define AT_CMD_xor_SPIDENTIFYSTART     146
#define AT_CMD_xor_SPIDENTIFYEND     147
#define AT_CMD_SPGPSPWR     148
#define AT_CMD_SPGPSRST     149
#define AT_CMD_SPGPSSTATUS     150
#define AT_CMD_SPGPSINF     151
#define AT_CMD_SPGPSOUT     152
#define AT_CMD_SPGPSFIX     153
#define AT_CMD_SPGPSPDP     154
#define AT_CMD_SPBLETEST     155
#define AT_CMD_QGNSSC     156
#define AT_CMD_QGNSSRD     157
#define AT_CMD_QGNSSCMD     158
#define AT_CMD_SPGPSTTFF     159
#define AT_CMD_FOTA     160
#define AT_CMD_SCWTEST     161
#define AT_CMD_xor_SCFG     162
#define AT_CMD_SPGPSASSERT     163
#define AT_CMD_SPGPSLTEOPEN     164
#define AT_CMD_CACCP     165
#define AT_CMD_CAVQE     166
#define AT_CMD_CAPRE     167
#define AT_CMD_CAPOST     168
#define AT_CMD_CAWTF     169
#define AT_CMD_CAIET     170
#define AT_CMD_CADTF     171
#define AT_CMD_CAVCT     172
#define AT_CMD_CAVIMS     173
#define AT_CMD_IMSEN     174
#define AT_CMD_SPSELMODE     175
#define AT_CMD_SPIMEI     176
#define AT_CMD_SFUN     177
#define AT_CMD_SPTEST     178
#define AT_CMD_SPTESTMODEM     179
#define AT_CMD_SPDATA     180
#define AT_CMD_CEMODE     181
#define AT_CMD_GLPDNS     182
#define AT_CMD_CGDSCONT     183
#define AT_CMD_CANXP     184
#define AT_CMD_CLIP     185
#define AT_CMD_CLIR     186
#define AT_CMD_COLP     187
#define AT_CMD_COLR     188
#define AT_CMD_SPACTCARD     189
#define AT_CMD_xor_SYSCONFIG     190
#define AT_CMD_SPLTEDUMMYPARA     191
#define AT_CMD_CCFCU     192
#define AT_CMD_RRTMPARAM     193
#define AT_CMD_GRRLTEFREQ     194
#define AT_CMD_L1PARAM     195
#define AT_CMD_RRDMPARAM     196
#define AT_CMD_SPCLEANINFO     197
#define AT_CMD_CGTFT     198
#define AT_CMD_CGEQOS     199
#define AT_CMD_CACM     200
#define AT_CMD_XCAPRTURI     201
#define AT_CMD_BSF     202
#define AT_CMD_CMMS     203
#define AT_CMD_CHUP     204
#define AT_CMD_CRC     205
#define AT_CMD_CRSM     206
#define AT_CMD_CISRVCC     207
#define AT_CMD_SPCHKSD     208
#define AT_CMD_CSSN     209
#define AT_CMD_EMMCDDRSIZE     210
#define AT_CMD_CHANGERAT     211
#define AT_CMD_CGSMS     212
#define AT_CMD_EVTS     213
#define AT_CMD_FACNETINFO     214
#define AT_CMD_FACNETINFOSTA     215
#define AT_CMD_SPRNDIS     216
#define AT_CMD_CHECKOPERAIN     217
#define AT_CMD_AUDCH     218
#define AT_CMD_SPWIQ     219
#define AT_CMD_IMSNV     220
#define AT_CMD_RRCREL     221
#define AT_CMD_SPSIMLOCKCHECK     222
#define AT_CMD_SPADCVS     223
#define AT_CMD_PEINFO     224
#define AT_CMD_WIFI     225
#define AT_CMD_PING     226
#define AT_CMD_SACK     227
#define AT_CMD_SOCKET     228
#define AT_CMD_HTTP     229
#define AT_CMD_NETIF     230
#define AT_CMD_MTU     231
#define AT_CMD_DAPSIPERF     232
#define AT_CMD_TRAFFIC     233
#define AT_CMD_MQTTSUBUNSUB     234
#define AT_CMD_MQTTCONN     235
#define AT_CMD_MQTTPUB     236
#define AT_CMD_MQTTDISCONN     237
#define AT_CMD_COAP_GET     238
#define AT_CMD_COAP_POST     239
#define AT_CMD_COAP_PUT     240
#define AT_CMD_COAP_DELETE     241
#define AT_CMD_ARMLOG     242
#define AT_CMD_TESTLWM2M     243
#define AT_CMD_PMMODE     244
#define AT_CMD_SPGPSTEST     245
#define AT_CMD_CGNSSST     246
#define AT_CMD_SDRMOD     247
#define AT_CMD_CALIBINFO     248

#endif /* End of #ifndef _ATC_DEF_H */
