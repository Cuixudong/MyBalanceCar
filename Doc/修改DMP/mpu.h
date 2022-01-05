#ifndef __MPU_H
#define __MPU_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
/* 换成你自己的平台头文件 */
#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DMP_PTAT    0
#define DMP_XGYR    2
#define DMP_YGYR    4
#define DMP_ZGYR    6
#define DMP_XACC    8
#define DMP_YACC    10
#define DMP_ZACC    12
#define DMP_ADC1    14
#define DMP_ADC2    16
#define DMP_ADC3    18
#define DMP_BIASUNC    20
#define DMP_FIFORT    22
#define DMP_INVGSFH    24
#define DMP_INVGSFL    26
#define DMP_1H    28
#define DMP_1L    30
#define DMP_BLPFSTCH    32
#define DMP_BLPFSTCL    34
#define DMP_BLPFSXH    36
#define DMP_BLPFSXL    38
#define DMP_BLPFSYH    40
#define DMP_BLPFSYL    42
#define DMP_BLPFSZH    44
#define DMP_BLPFSZL    46
#define DMP_BLPFMTC    48
#define DMP_SMC    50
#define DMP_BLPFMXH    52
#define DMP_BLPFMXL    54
#define DMP_BLPFMYH    56
#define DMP_BLPFMYL    58
#define DMP_BLPFMZH    60
#define DMP_BLPFMZL    62
#define DMP_BLPFC    64
#define DMP_SMCTH    66
#define DMP_0H2    68
#define DMP_0L2    70
#define DMP_BERR2H    72
#define DMP_BERR2L    74
#define DMP_BERR2NH    76
#define DMP_SMCINC    78
#define DMP_ANGVBXH    80
#define DMP_ANGVBXL    82
#define DMP_ANGVBYH    84
#define DMP_ANGVBYL    86
#define DMP_ANGVBZH    88
#define DMP_ANGVBZL    90
#define DMP_BERR1H    92
#define DMP_BERR1L    94
#define DMP_ATCH    96
#define DMP_BIASUNCSF    98
#define DMP_ACT2H    100
#define DMP_ACT2L    102
#define DMP_GSFH    104
#define DMP_GSFL    106
#define DMP_GH    108
#define DMP_GL    110
#define DMP_0_5H    112
#define DMP_0_5L    114
#define DMP_0_0H    116
#define DMP_0_0L    118
#define DMP_1_0H    120
#define DMP_1_0L    122
#define DMP_1_5H    124
#define DMP_1_5L    126
#define DMP_TMP1AH    128
#define DMP_TMP1AL    130
#define DMP_TMP2AH    132
#define DMP_TMP2AL    134
#define DMP_TMP3AH    136
#define DMP_TMP3AL    138
#define DMP_TMP4AH    140
#define DMP_TMP4AL    142
#define DMP_XACCW    144
#define DMP_TMP5    146
#define DMP_XACCB    148
#define DMP_TMP8    150
#define DMP_YACCB    152
#define DMP_TMP9    154
#define DMP_ZACCB    156
#define DMP_TMP10    158
#define DMP_DZH    160
#define DMP_DZL    162
#define DMP_XGCH    164
#define DMP_XGCL    166
#define DMP_YGCH    168
#define DMP_YGCL    170
#define DMP_ZGCH    172
#define DMP_ZGCL    174
#define DMP_YACCW    176
#define DMP_TMP7    178
#define DMP_AFB1H    180
#define DMP_AFB1L    182
#define DMP_AFB2H    184
#define DMP_AFB2L    186
#define DMP_MAGFBH    188
#define DMP_MAGFBL    190
#define DMP_QT1H    192
#define DMP_QT1L    194
#define DMP_QT2H    196
#define DMP_QT2L    198
#define DMP_QT3H    200
#define DMP_QT3L    202
#define DMP_QT4H    204
#define DMP_QT4L    206
#define DMP_CTRL1H    208
#define DMP_CTRL1L    210
#define DMP_CTRL2H    212
#define DMP_CTRL2L    214
#define DMP_CTRL3H    216
#define DMP_CTRL3L    218
#define DMP_CTRL4H    220
#define DMP_CTRL4L    222
#define DMP_CTRLS1    224
#define DMP_CTRLSF1    226
#define DMP_CTRLS2    228
#define DMP_CTRLSF2    230
#define DMP_CTRLS3    232
#define DMP_CTRLSFNLL    234
#define DMP_CTRLS4    236
#define DMP_CTRLSFNL2    238
#define DMP_CTRLSFNL    240
#define DMP_TMP30    242
#define DMP_CTRLSFJT    244
#define DMP_TMP31    246
#define DMP_TMP11    248
#define DMP_CTRLSF2_2    250
#define DMP_TMP12    252
#define DMP_CTRLSF1_2    254
#define DMP_PREVPTAT    256
#define DMP_ACCZB    258
#define DMP_ACCXB    264
#define DMP_ACCYB    266
#define DMP_1HB    272
#define DMP_1LB    274
#define DMP_0H    276
#define DMP_0L    278
#define DMP_ASR22H    280
#define DMP_ASR22L    282
#define DMP_ASR6H    284
#define DMP_ASR6L    286
#define DMP_TMP13    288
#define DMP_TMP14    290
#define DMP_FINTXH    292
#define DMP_FINTXL    294
#define DMP_FINTYH    296
#define DMP_FINTYL    298
#define DMP_FINTZH    300
#define DMP_FINTZL    302
#define DMP_TMP1BH    304
#define DMP_TMP1BL    306
#define DMP_TMP2BH    308
#define DMP_TMP2BL    310
#define DMP_TMP3BH    312
#define DMP_TMP3BL    314
#define DMP_TMP4BH    316
#define DMP_TMP4BL    318
#define DMP_STXG    320
#define DMP_ZCTXG    322
#define DMP_STYG    324
#define DMP_ZCTYG    326
#define DMP_STZG    328
#define DMP_ZCTZG    330
#define DMP_CTRLSFJT2    332
#define DMP_CTRLSFJTCNT    334
#define DMP_PVXG    336
#define DMP_TMP15    338
#define DMP_PVYG    340
#define DMP_TMP16    342
#define DMP_PVZG    344
#define DMP_TMP17    346
#define DMP_MNMFLAGH    352
#define DMP_MNMFLAGL    354
#define DMP_MNMTMH    356
#define DMP_MNMTML    358
#define DMP_MNMTMTHRH    360
#define DMP_MNMTMTHRL    362
#define DMP_MNMTHRH    364
#define DMP_MNMTHRL    366
#define DMP_ACCQD4H    368
#define DMP_ACCQD4L    370
#define DMP_ACCQD5H    372
#define DMP_ACCQD5L    374
#define DMP_ACCQD6H    376
#define DMP_ACCQD6L    378
#define DMP_ACCQD7H    380
#define DMP_ACCQD7L    382
#define DMP_ACCQD0H    384
#define DMP_ACCQD0L    386
#define DMP_ACCQD1H    388
#define DMP_ACCQD1L    390
#define DMP_ACCQD2H    392
#define DMP_ACCQD2L    394
#define DMP_ACCQD3H    396
#define DMP_ACCQD3L    398
#define DMP_XN2H    400
#define DMP_XN2L    402
#define DMP_XN1H    404
#define DMP_XN1L    406
#define DMP_YN2H    408
#define DMP_YN2L    410
#define DMP_YN1H    412
#define DMP_YN1L    414
#define DMP_YH    416
#define DMP_YL    418
#define DMP_B0H    420
#define DMP_B0L    422
#define DMP_A1H    424
#define DMP_A1L    426
#define DMP_A2H    428
#define DMP_A2L    430
#define DMP_SEM1    432
#define DMP_FIFOCNT    434
#define DMP_SH_TH_X    436
#define DMP_PACKET    438
#define DMP_SH_TH_Y    440
#define DMP_FOOTER    442
#define DMP_SH_TH_Z    444
#define DMP_TEMP29    448
#define DMP_TEMP30    450
#define DMP_XACCB_PRE    452
#define DMP_XACCB_PREL    454
#define DMP_YACCB_PRE    456
#define DMP_YACCB_PREL    458
#define DMP_ZACCB_PRE    460
#define DMP_ZACCB_PREL    462
#define DMP_TMP22    464
#define DMP_TAP_TIMER    466
#define DMP_TAP_THX    468
#define DMP_TAP_THY    472
#define DMP_TAP_THZ    476
#define DMP_TAPW_MIN    478
#define DMP_TMP25    480
#define DMP_TMP26    482
#define DMP_TMP27    484
#define DMP_TMP28    486
#define DMP_ORIENT    488
#define DMP_THRSH    490
#define DMP_ENDIANH    492
#define DMP_ENDIANL    494
#define DMP_BLPFNMTCH    496
#define DMP_BLPFNMTCL    498
#define DMP_BLPFNMXH    500
#define DMP_BLPFNMXL    502
#define DMP_BLPFNMYH    504
#define DMP_BLPFNMYL    506
#define DMP_BLPFNMZH    508
#define DMP_BLPFNMZL    510


#define KEY_CFG_25                  (0)
#define KEY_CFG_24                  (KEY_CFG_25 + 1)
#define KEY_CFG_26                  (KEY_CFG_24 + 1)
#define KEY_CFG_27                  (KEY_CFG_26 + 1)
#define KEY_CFG_21                  (KEY_CFG_27 + 1)
#define KEY_CFG_20                  (KEY_CFG_21 + 1)
#define KEY_CFG_TAP4                (KEY_CFG_20 + 1)
#define KEY_CFG_TAP5                (KEY_CFG_TAP4 + 1)
#define KEY_CFG_TAP6                (KEY_CFG_TAP5 + 1)
#define KEY_CFG_TAP7                (KEY_CFG_TAP6 + 1)
#define KEY_CFG_TAP0                (KEY_CFG_TAP7 + 1)
#define KEY_CFG_TAP1                (KEY_CFG_TAP0 + 1)
#define KEY_CFG_TAP2                (KEY_CFG_TAP1 + 1)
#define KEY_CFG_TAP3                (KEY_CFG_TAP2 + 1)
#define KEY_CFG_TAP_QUANTIZE        (KEY_CFG_TAP3 + 1)
#define KEY_CFG_TAP_JERK            (KEY_CFG_TAP_QUANTIZE + 1)
#define KEY_CFG_DR_INT              (KEY_CFG_TAP_JERK + 1)
#define KEY_CFG_AUTH                (KEY_CFG_DR_INT + 1)
#define KEY_CFG_TAP_SAVE_ACCB       (KEY_CFG_AUTH + 1)
#define KEY_CFG_TAP_CLEAR_STICKY    (KEY_CFG_TAP_SAVE_ACCB + 1)
#define KEY_CFG_FIFO_ON_EVENT       (KEY_CFG_TAP_CLEAR_STICKY + 1)
#define KEY_FCFG_ACCEL_INPUT        (KEY_CFG_FIFO_ON_EVENT + 1)
#define KEY_FCFG_ACCEL_INIT         (KEY_FCFG_ACCEL_INPUT + 1)
#define KEY_CFG_23                  (KEY_FCFG_ACCEL_INIT + 1)
#define KEY_FCFG_1                  (KEY_CFG_23 + 1)
#define KEY_FCFG_3                  (KEY_FCFG_1 + 1)
#define KEY_FCFG_2                  (KEY_FCFG_3 + 1)
#define KEY_CFG_3D                  (KEY_FCFG_2 + 1)
#define KEY_CFG_3B                  (KEY_CFG_3D + 1)
#define KEY_CFG_3C                  (KEY_CFG_3B + 1)
#define KEY_FCFG_5                  (KEY_CFG_3C + 1)
#define KEY_FCFG_4                  (KEY_FCFG_5 + 1)
#define KEY_FCFG_7                  (KEY_FCFG_4 + 1)
#define KEY_FCFG_FSCALE             (KEY_FCFG_7 + 1)
#define KEY_FCFG_AZ                 (KEY_FCFG_FSCALE + 1)
#define KEY_FCFG_6                  (KEY_FCFG_AZ + 1)
#define KEY_FCFG_LSB4               (KEY_FCFG_6 + 1)
#define KEY_CFG_12                  (KEY_FCFG_LSB4 + 1)
#define KEY_CFG_14                  (KEY_CFG_12 + 1)
#define KEY_CFG_15                  (KEY_CFG_14 + 1)
#define KEY_CFG_16                  (KEY_CFG_15 + 1)
#define KEY_CFG_18                  (KEY_CFG_16 + 1)
#define KEY_CFG_6                   (KEY_CFG_18 + 1)
#define KEY_CFG_7                   (KEY_CFG_6 + 1)
#define KEY_CFG_4                   (KEY_CFG_7 + 1)
#define KEY_CFG_5                   (KEY_CFG_4 + 1)
#define KEY_CFG_2                   (KEY_CFG_5 + 1)
#define KEY_CFG_3                   (KEY_CFG_2 + 1)
#define KEY_CFG_1                   (KEY_CFG_3 + 1)
#define KEY_CFG_EXTERNAL            (KEY_CFG_1 + 1)
#define KEY_CFG_8                   (KEY_CFG_EXTERNAL + 1)
#define KEY_CFG_9                   (KEY_CFG_8 + 1)
#define KEY_CFG_ORIENT_3            (KEY_CFG_9 + 1)
#define KEY_CFG_ORIENT_2            (KEY_CFG_ORIENT_3 + 1)
#define KEY_CFG_ORIENT_1            (KEY_CFG_ORIENT_2 + 1)
#define KEY_CFG_GYRO_SOURCE         (KEY_CFG_ORIENT_1 + 1)
#define KEY_CFG_ORIENT_IRQ_1        (KEY_CFG_GYRO_SOURCE + 1)
#define KEY_CFG_ORIENT_IRQ_2        (KEY_CFG_ORIENT_IRQ_1 + 1)
#define KEY_CFG_ORIENT_IRQ_3        (KEY_CFG_ORIENT_IRQ_2 + 1)
#define KEY_FCFG_MAG_VAL            (KEY_CFG_ORIENT_IRQ_3 + 1)
#define KEY_FCFG_MAG_MOV            (KEY_FCFG_MAG_VAL + 1)
#define KEY_CFG_LP_QUAT             (KEY_FCFG_MAG_MOV + 1)

/* MPU6050 keys */
#define KEY_CFG_ACCEL_FILTER        (KEY_CFG_LP_QUAT + 1)
#define KEY_CFG_MOTION_BIAS         (KEY_CFG_ACCEL_FILTER + 1)
#define KEY_TEMPLABEL               (KEY_CFG_MOTION_BIAS + 1)

#define KEY_D_0_22                  (KEY_TEMPLABEL + 1)
#define KEY_D_0_24                  (KEY_D_0_22 + 1)
#define KEY_D_0_36                  (KEY_D_0_24 + 1)
#define KEY_D_0_52                  (KEY_D_0_36 + 1)
#define KEY_D_0_96                  (KEY_D_0_52 + 1)
#define KEY_D_0_104                 (KEY_D_0_96 + 1)
#define KEY_D_0_108                 (KEY_D_0_104 + 1)
#define KEY_D_0_163                 (KEY_D_0_108 + 1)
#define KEY_D_0_188                 (KEY_D_0_163 + 1)
#define KEY_D_0_192                 (KEY_D_0_188 + 1)
#define KEY_D_0_224                 (KEY_D_0_192 + 1)
#define KEY_D_0_228                 (KEY_D_0_224 + 1)
#define KEY_D_0_232                 (KEY_D_0_228 + 1)
#define KEY_D_0_236                 (KEY_D_0_232 + 1)

#define KEY_DMP_PREVPTAT            (KEY_D_0_236 + 1)
#define KEY_D_1_2                   (KEY_DMP_PREVPTAT + 1)
#define KEY_D_1_4                   (KEY_D_1_2 + 1)
#define KEY_D_1_8                   (KEY_D_1_4 + 1)
#define KEY_D_1_10                  (KEY_D_1_8 + 1)
#define KEY_D_1_24                  (KEY_D_1_10 + 1)
#define KEY_D_1_28                  (KEY_D_1_24 + 1)
#define KEY_D_1_36                  (KEY_D_1_28 + 1)
#define KEY_D_1_40                  (KEY_D_1_36 + 1)
#define KEY_D_1_44                  (KEY_D_1_40 + 1)
#define KEY_D_1_72                  (KEY_D_1_44 + 1)
#define KEY_D_1_74                  (KEY_D_1_72 + 1)
#define KEY_D_1_79                  (KEY_D_1_74 + 1)
#define KEY_D_1_88                  (KEY_D_1_79 + 1)
#define KEY_D_1_90                  (KEY_D_1_88 + 1)
#define KEY_D_1_92                  (KEY_D_1_90 + 1)
#define KEY_D_1_96                  (KEY_D_1_92 + 1)
#define KEY_D_1_98                  (KEY_D_1_96 + 1)
#define KEY_D_1_100                 (KEY_D_1_98 + 1)
#define KEY_D_1_106                 (KEY_D_1_100 + 1)
#define KEY_D_1_108                 (KEY_D_1_106 + 1)
#define KEY_D_1_112                 (KEY_D_1_108 + 1)
#define KEY_D_1_128                 (KEY_D_1_112 + 1)
#define KEY_D_1_152                 (KEY_D_1_128 + 1)
#define KEY_D_1_160                 (KEY_D_1_152 + 1)
#define KEY_D_1_168                 (KEY_D_1_160 + 1)
#define KEY_D_1_175                 (KEY_D_1_168 + 1)
#define KEY_D_1_176                 (KEY_D_1_175 + 1)
#define KEY_D_1_178                 (KEY_D_1_176 + 1)
#define KEY_D_1_179                 (KEY_D_1_178 + 1)
#define KEY_D_1_218                 (KEY_D_1_179 + 1)
#define KEY_D_1_232                 (KEY_D_1_218 + 1)
#define KEY_D_1_236                 (KEY_D_1_232 + 1)
#define KEY_D_1_240                 (KEY_D_1_236 + 1)
#define KEY_D_1_244                 (KEY_D_1_240 + 1)
#define KEY_D_1_250                 (KEY_D_1_244 + 1)
#define KEY_D_1_252                 (KEY_D_1_250 + 1)
#define KEY_D_2_12                  (KEY_D_1_252 + 1)
#define KEY_D_2_96                  (KEY_D_2_12 + 1)
#define KEY_D_2_108                 (KEY_D_2_96 + 1)
#define KEY_D_2_208                 (KEY_D_2_108 + 1)
#define KEY_FLICK_MSG               (KEY_D_2_208 + 1)
#define KEY_FLICK_COUNTER           (KEY_FLICK_MSG + 1)
#define KEY_FLICK_LOWER             (KEY_FLICK_COUNTER + 1)
#define KEY_CFG_FLICK_IN            (KEY_FLICK_LOWER + 1)
#define KEY_FLICK_UPPER             (KEY_CFG_FLICK_IN + 1)
#define KEY_CGNOTICE_INTR           (KEY_FLICK_UPPER + 1)
#define KEY_D_2_224                 (KEY_CGNOTICE_INTR + 1)
#define KEY_D_2_244                 (KEY_D_2_224 + 1)
#define KEY_D_2_248                 (KEY_D_2_244 + 1)
#define KEY_D_2_252                 (KEY_D_2_248 + 1)

#define KEY_D_GYRO_BIAS_X               (KEY_D_2_252 + 1)
#define KEY_D_GYRO_BIAS_Y               (KEY_D_GYRO_BIAS_X + 1)
#define KEY_D_GYRO_BIAS_Z               (KEY_D_GYRO_BIAS_Y + 1)
#define KEY_D_ACC_BIAS_X                (KEY_D_GYRO_BIAS_Z + 1)
#define KEY_D_ACC_BIAS_Y                (KEY_D_ACC_BIAS_X + 1)
#define KEY_D_ACC_BIAS_Z                (KEY_D_ACC_BIAS_Y + 1)
#define KEY_D_GYRO_ENABLE               (KEY_D_ACC_BIAS_Z + 1)
#define KEY_D_ACCEL_ENABLE              (KEY_D_GYRO_ENABLE + 1)
#define KEY_D_QUAT_ENABLE               (KEY_D_ACCEL_ENABLE +1)
#define KEY_D_OUTPUT_ENABLE             (KEY_D_QUAT_ENABLE + 1)
#define KEY_D_CR_TIME_G                 (KEY_D_OUTPUT_ENABLE + 1)
#define KEY_D_CR_TIME_A                 (KEY_D_CR_TIME_G + 1)
#define KEY_D_CR_TIME_Q                 (KEY_D_CR_TIME_A + 1)
#define KEY_D_CS_TAX                    (KEY_D_CR_TIME_Q + 1)
#define KEY_D_CS_TAY                    (KEY_D_CS_TAX + 1)
#define KEY_D_CS_TAZ                    (KEY_D_CS_TAY + 1)
#define KEY_D_CS_TGX                    (KEY_D_CS_TAZ + 1)
#define KEY_D_CS_TGY                    (KEY_D_CS_TGX + 1)
#define KEY_D_CS_TGZ                    (KEY_D_CS_TGY + 1)
#define KEY_D_CS_TQ0                    (KEY_D_CS_TGZ + 1)
#define KEY_D_CS_TQ1                    (KEY_D_CS_TQ0 + 1)
#define KEY_D_CS_TQ2                    (KEY_D_CS_TQ1 + 1)
#define KEY_D_CS_TQ3                    (KEY_D_CS_TQ2 + 1)

/* Compass keys */
#define KEY_CPASS_BIAS_X            (KEY_D_CS_TQ3 + 1)
#define KEY_CPASS_BIAS_Y            (KEY_CPASS_BIAS_X + 1)
#define KEY_CPASS_BIAS_Z            (KEY_CPASS_BIAS_Y + 1)
#define KEY_CPASS_MTX_00            (KEY_CPASS_BIAS_Z + 1)
#define KEY_CPASS_MTX_01            (KEY_CPASS_MTX_00 + 1)
#define KEY_CPASS_MTX_02            (KEY_CPASS_MTX_01 + 1)
#define KEY_CPASS_MTX_10            (KEY_CPASS_MTX_02 + 1)
#define KEY_CPASS_MTX_11            (KEY_CPASS_MTX_10 + 1)
#define KEY_CPASS_MTX_12            (KEY_CPASS_MTX_11 + 1)
#define KEY_CPASS_MTX_20            (KEY_CPASS_MTX_12 + 1)
#define KEY_CPASS_MTX_21            (KEY_CPASS_MTX_20 + 1)
#define KEY_CPASS_MTX_22            (KEY_CPASS_MTX_21 + 1)

/* Gesture Keys */
#define KEY_DMP_TAPW_MIN            (KEY_CPASS_MTX_22 + 1)
#define KEY_DMP_TAP_THR_X           (KEY_DMP_TAPW_MIN + 1)
#define KEY_DMP_TAP_THR_Y           (KEY_DMP_TAP_THR_X + 1)
#define KEY_DMP_TAP_THR_Z           (KEY_DMP_TAP_THR_Y + 1)
#define KEY_DMP_SH_TH_Y             (KEY_DMP_TAP_THR_Z + 1)
#define KEY_DMP_SH_TH_X             (KEY_DMP_SH_TH_Y + 1)
#define KEY_DMP_SH_TH_Z             (KEY_DMP_SH_TH_X + 1)
#define KEY_DMP_ORIENT              (KEY_DMP_SH_TH_Z + 1)
#define KEY_D_ACT0                  (KEY_DMP_ORIENT + 1)
#define KEY_D_ACSX                  (KEY_D_ACT0 + 1)
#define KEY_D_ACSY                  (KEY_D_ACSX + 1)
#define KEY_D_ACSZ                  (KEY_D_ACSY + 1)

#define KEY_X_GRT_Y_TMP             (KEY_D_ACSZ + 1)
#define KEY_SKIP_X_GRT_Y_TMP        (KEY_X_GRT_Y_TMP + 1)
#define KEY_SKIP_END_COMPARE        (KEY_SKIP_X_GRT_Y_TMP + 1)
#define KEY_END_COMPARE_Y_X_TMP2    (KEY_SKIP_END_COMPARE + 1)
#define KEY_CFG_ANDROID_ORIENT_INT  (KEY_END_COMPARE_Y_X_TMP2 + 1)
#define KEY_NO_ORIENT_INTERRUPT     (KEY_CFG_ANDROID_ORIENT_INT + 1)
#define KEY_END_COMPARE_Y_X_TMP     (KEY_NO_ORIENT_INTERRUPT + 1)
#define KEY_END_ORIENT_1            (KEY_END_COMPARE_Y_X_TMP + 1)
#define KEY_END_COMPARE_Y_X         (KEY_END_ORIENT_1 + 1)
#define KEY_END_ORIENT              (KEY_END_COMPARE_Y_X + 1)
#define KEY_X_GRT_Y                 (KEY_END_ORIENT + 1)
#define KEY_NOT_TIME_MINUS_1        (KEY_X_GRT_Y + 1)
#define KEY_END_COMPARE_Y_X_TMP3    (KEY_NOT_TIME_MINUS_1 + 1)
#define KEY_X_GRT_Y_TMP2            (KEY_END_COMPARE_Y_X_TMP3 + 1)

/* Authenticate Keys */
#define KEY_D_AUTH_OUT              (KEY_X_GRT_Y_TMP2 + 1)
#define KEY_D_AUTH_IN               (KEY_D_AUTH_OUT + 1)
#define KEY_D_AUTH_A                (KEY_D_AUTH_IN + 1)
#define KEY_D_AUTH_B                (KEY_D_AUTH_A + 1)

/* Pedometer standalone only keys */
#define KEY_D_PEDSTD_BP_B           (KEY_D_AUTH_B + 1)
#define KEY_D_PEDSTD_HP_A           (KEY_D_PEDSTD_BP_B + 1)
#define KEY_D_PEDSTD_HP_B           (KEY_D_PEDSTD_HP_A + 1)
#define KEY_D_PEDSTD_BP_A4          (KEY_D_PEDSTD_HP_B + 1)
#define KEY_D_PEDSTD_BP_A3          (KEY_D_PEDSTD_BP_A4 + 1)
#define KEY_D_PEDSTD_BP_A2          (KEY_D_PEDSTD_BP_A3 + 1)
#define KEY_D_PEDSTD_BP_A1          (KEY_D_PEDSTD_BP_A2 + 1)
#define KEY_D_PEDSTD_INT_THRSH      (KEY_D_PEDSTD_BP_A1 + 1)
#define KEY_D_PEDSTD_CLIP           (KEY_D_PEDSTD_INT_THRSH + 1)
#define KEY_D_PEDSTD_SB             (KEY_D_PEDSTD_CLIP + 1)
#define KEY_D_PEDSTD_SB_TIME        (KEY_D_PEDSTD_SB + 1)
#define KEY_D_PEDSTD_PEAKTHRSH      (KEY_D_PEDSTD_SB_TIME + 1)
#define KEY_D_PEDSTD_TIML           (KEY_D_PEDSTD_PEAKTHRSH + 1)
#define KEY_D_PEDSTD_TIMH           (KEY_D_PEDSTD_TIML + 1)
#define KEY_D_PEDSTD_PEAK           (KEY_D_PEDSTD_TIMH + 1)
#define KEY_D_PEDSTD_TIMECTR        (KEY_D_PEDSTD_PEAK + 1)
#define KEY_D_PEDSTD_STEPCTR        (KEY_D_PEDSTD_TIMECTR + 1)
#define KEY_D_PEDSTD_WALKTIME       (KEY_D_PEDSTD_STEPCTR + 1)
#define KEY_D_PEDSTD_DECI           (KEY_D_PEDSTD_WALKTIME + 1)

/*Host Based No Motion*/
#define KEY_D_HOST_NO_MOT           (KEY_D_PEDSTD_DECI + 1)

/* EIS keys */
#define KEY_P_EIS_FIFO_FOOTER       (KEY_D_HOST_NO_MOT + 1)
#define KEY_P_EIS_FIFO_YSHIFT       (KEY_P_EIS_FIFO_FOOTER + 1)
#define KEY_P_EIS_DATA_RATE         (KEY_P_EIS_FIFO_YSHIFT + 1)
#define KEY_P_EIS_FIFO_XSHIFT       (KEY_P_EIS_DATA_RATE + 1)
#define KEY_P_EIS_FIFO_SYNC         (KEY_P_EIS_FIFO_XSHIFT + 1)
#define KEY_P_EIS_FIFO_ZSHIFT       (KEY_P_EIS_FIFO_SYNC + 1)
#define KEY_P_EIS_FIFO_READY        (KEY_P_EIS_FIFO_ZSHIFT + 1)
#define KEY_DMP_FOOTER              (KEY_P_EIS_FIFO_READY + 1)
#define KEY_DMP_INTX_HC             (KEY_DMP_FOOTER + 1)
#define KEY_DMP_INTX_PH             (KEY_DMP_INTX_HC + 1)
#define KEY_DMP_INTX_SH             (KEY_DMP_INTX_PH + 1)
#define KEY_DMP_AINV_SH             (KEY_DMP_INTX_SH + 1)
#define KEY_DMP_A_INV_XH            (KEY_DMP_AINV_SH + 1)
#define KEY_DMP_AINV_PH             (KEY_DMP_A_INV_XH + 1)
#define KEY_DMP_CTHX_H              (KEY_DMP_AINV_PH + 1)
#define KEY_DMP_CTHY_H              (KEY_DMP_CTHX_H + 1)
#define KEY_DMP_CTHZ_H              (KEY_DMP_CTHY_H + 1)
#define KEY_DMP_NCTHX_H             (KEY_DMP_CTHZ_H + 1)
#define KEY_DMP_NCTHY_H             (KEY_DMP_NCTHX_H + 1)
#define KEY_DMP_NCTHZ_H             (KEY_DMP_NCTHY_H + 1)
#define KEY_DMP_CTSQ_XH             (KEY_DMP_NCTHZ_H + 1)
#define KEY_DMP_CTSQ_YH             (KEY_DMP_CTSQ_XH + 1)
#define KEY_DMP_CTSQ_ZH             (KEY_DMP_CTSQ_YH + 1)
#define KEY_DMP_INTX_H              (KEY_DMP_CTSQ_ZH + 1)
#define KEY_DMP_INTY_H              (KEY_DMP_INTX_H + 1)
#define KEY_DMP_INTZ_H              (KEY_DMP_INTY_H + 1)
//#define KEY_DMP_HPX_H               (KEY_DMP_INTZ_H + 1)
//#define KEY_DMP_HPY_H               (KEY_DMP_HPX_H + 1)
//#define KEY_DMP_HPZ_H               (KEY_DMP_HPY_H + 1)

/* Stream keys */
#define KEY_STREAM_P_GYRO_Z         (KEY_DMP_INTZ_H + 1)
#define KEY_STREAM_P_GYRO_Y         (KEY_STREAM_P_GYRO_Z + 1)
#define KEY_STREAM_P_GYRO_X         (KEY_STREAM_P_GYRO_Y + 1)
#define KEY_STREAM_P_TEMP           (KEY_STREAM_P_GYRO_X + 1)
#define KEY_STREAM_P_AUX_Y          (KEY_STREAM_P_TEMP + 1)
#define KEY_STREAM_P_AUX_X          (KEY_STREAM_P_AUX_Y + 1)
#define KEY_STREAM_P_AUX_Z          (KEY_STREAM_P_AUX_X + 1)
#define KEY_STREAM_P_ACCEL_Y        (KEY_STREAM_P_AUX_Z + 1)
#define KEY_STREAM_P_ACCEL_X        (KEY_STREAM_P_ACCEL_Y + 1)
#define KEY_STREAM_P_FOOTER         (KEY_STREAM_P_ACCEL_X + 1)
#define KEY_STREAM_P_ACCEL_Z        (KEY_STREAM_P_FOOTER + 1)

#define NUM_KEYS                    (KEY_STREAM_P_ACCEL_Z + 1)

typedef struct {
    unsigned short key;
    unsigned short addr;
} tKeyLabel;

#define DINA0A 0x0a
#define DINA22 0x22
#define DINA42 0x42
#define DINA5A 0x5a

#define DINA06 0x06
#define DINA0E 0x0e
#define DINA16 0x16
#define DINA1E 0x1e
#define DINA26 0x26
#define DINA2E 0x2e
#define DINA36 0x36
#define DINA3E 0x3e
#define DINA46 0x46
#define DINA4E 0x4e
#define DINA56 0x56
#define DINA5E 0x5e
#define DINA66 0x66
#define DINA6E 0x6e
#define DINA76 0x76
#define DINA7E 0x7e

#define DINA00 0x00
#define DINA08 0x08
#define DINA10 0x10
#define DINA18 0x18
#define DINA20 0x20
#define DINA28 0x28
#define DINA30 0x30
#define DINA38 0x38
#define DINA40 0x40
#define DINA48 0x48
#define DINA50 0x50
#define DINA58 0x58
#define DINA60 0x60
#define DINA68 0x68
#define DINA70 0x70
#define DINA78 0x78

#define DINA04 0x04
#define DINA0C 0x0c
#define DINA14 0x14
#define DINA1C 0x1C
#define DINA24 0x24
#define DINA2C 0x2c
#define DINA34 0x34
#define DINA3C 0x3c
#define DINA44 0x44
#define DINA4C 0x4c
#define DINA54 0x54
#define DINA5C 0x5c
#define DINA64 0x64
#define DINA6C 0x6c
#define DINA74 0x74
#define DINA7C 0x7c

#define DINA01 0x01
#define DINA09 0x09
#define DINA11 0x11
#define DINA19 0x19
#define DINA21 0x21
#define DINA29 0x29
#define DINA31 0x31
#define DINA39 0x39
#define DINA41 0x41
#define DINA49 0x49
#define DINA51 0x51
#define DINA59 0x59
#define DINA61 0x61
#define DINA69 0x69
#define DINA71 0x71
#define DINA79 0x79

#define DINA25 0x25
#define DINA2D 0x2d
#define DINA35 0x35
#define DINA3D 0x3d
#define DINA4D 0x4d
#define DINA55 0x55
#define DINA5D 0x5D
#define DINA6D 0x6d
#define DINA75 0x75
#define DINA7D 0x7d

#define DINADC 0xdc
#define DINAF2 0xf2
#define DINAAB 0xab
#define DINAAA 0xaa
#define DINAF1 0xf1
#define DINADF 0xdf
#define DINADA 0xda
#define DINAB1 0xb1
#define DINAB9 0xb9
#define DINAF3 0xf3
#define DINA8B 0x8b
#define DINAA3 0xa3
#define DINA91 0x91
#define DINAB6 0xb6
#define DINAB4 0xb4

#define DINC00 0x00
#define DINC01 0x01
#define DINC02 0x02
#define DINC03 0x03
#define DINC08 0x08
#define DINC09 0x09
#define DINC0A 0x0a
#define DINC0B 0x0b
#define DINC10 0x10
#define DINC11 0x11
#define DINC12 0x12
#define DINC13 0x13
#define DINC18 0x18
#define DINC19 0x19
#define DINC1A 0x1a
#define DINC1B 0x1b

#define DINC20 0x20
#define DINC21 0x21
#define DINC22 0x22
#define DINC23 0x23
#define DINC28 0x28
#define DINC29 0x29
#define DINC2A 0x2a
#define DINC2B 0x2b
#define DINC30 0x30
#define DINC31 0x31
#define DINC32 0x32
#define DINC33 0x33
#define DINC38 0x38
#define DINC39 0x39
#define DINC3A 0x3a
#define DINC3B 0x3b

#define DINC40 0x40
#define DINC41 0x41
#define DINC42 0x42
#define DINC43 0x43
#define DINC48 0x48
#define DINC49 0x49
#define DINC4A 0x4a
#define DINC4B 0x4b
#define DINC50 0x50
#define DINC51 0x51
#define DINC52 0x52
#define DINC53 0x53
#define DINC58 0x58
#define DINC59 0x59
#define DINC5A 0x5a
#define DINC5B 0x5b

#define DINC60 0x60
#define DINC61 0x61
#define DINC62 0x62
#define DINC63 0x63
#define DINC68 0x68
#define DINC69 0x69
#define DINC6A 0x6a
#define DINC6B 0x6b
#define DINC70 0x70
#define DINC71 0x71
#define DINC72 0x72
#define DINC73 0x73
#define DINC78 0x78
#define DINC79 0x79
#define DINC7A 0x7a
#define DINC7B 0x7b

#define DIND40 0x40

#define DINA80 0x80
#define DINA90 0x90
#define DINAA0 0xa0
#define DINAC9 0xc9
#define DINACB 0xcb
#define DINACD 0xcd
#define DINACF 0xcf
#define DINAC8 0xc8
#define DINACA 0xca
#define DINACC 0xcc
#define DINACE 0xce
#define DINAD8 0xd8
#define DINADD 0xdd
#define DINAF8 0xf0
#define DINAFE 0xfe

#define DINBF8 0xf8
#define DINAC0 0xb0
#define DINAC1 0xb1
#define DINAC2 0xb4
#define DINAC3 0xb5
#define DINAC4 0xb8
#define DINAC5 0xb9
#define DINBC0 0xc0
#define DINBC2 0xc2
#define DINBC4 0xc4
#define DINBC6 0xc6


#define TAP_X               (0x01)
#define TAP_Y               (0x02)
#define TAP_Z               (0x04)
#define TAP_XYZ             (0x07)

#define TAP_X_UP            (0x01)
#define TAP_X_DOWN          (0x02)
#define TAP_Y_UP            (0x03)
#define TAP_Y_DOWN          (0x04)
#define TAP_Z_UP            (0x05)
#define TAP_Z_DOWN          (0x06)

#define ANDROID_ORIENT_PORTRAIT             (0x00)
#define ANDROID_ORIENT_LANDSCAPE            (0x01)
#define ANDROID_ORIENT_REVERSE_PORTRAIT     (0x02)
#define ANDROID_ORIENT_REVERSE_LANDSCAPE    (0x03)

#define DMP_INT_GESTURE     (0x01)
#define DMP_INT_CONTINUOUS  (0x02)

#define DMP_FEATURE_TAP             (0x001)
#define DMP_FEATURE_ANDROID_ORIENT  (0x002)
#define DMP_FEATURE_LP_QUAT         (0x004)
#define DMP_FEATURE_PEDOMETER       (0x008)
#define DMP_FEATURE_6X_LP_QUAT      (0x010)
#define DMP_FEATURE_GYRO_CAL        (0x020)
#define DMP_FEATURE_SEND_RAW_ACCEL  (0x040)
#define DMP_FEATURE_SEND_RAW_GYRO   (0x080)
#define DMP_FEATURE_SEND_CAL_GYRO   (0x100)

#define INV_WXYZ_QUAT       (0x100)

/* Set up functions. */
int dmp_load_motion_driver_firmware(void);
int dmp_set_fifo_rate(unsigned short rate);
int dmp_get_fifo_rate(unsigned short *rate);
int dmp_enable_feature(unsigned short mask);
int dmp_get_enabled_features(unsigned short *mask);
int dmp_set_interrupt_mode(unsigned char mode);
int dmp_set_orientation(unsigned short orient);
int dmp_set_gyro_bias(long *bias);
int dmp_set_accel_bias(long *bias);

/* Tap functions. */
int dmp_register_tap_cb(void (*func)(unsigned char, unsigned char));
int dmp_set_tap_thresh(unsigned char axis, unsigned short thresh);
int dmp_set_tap_axes(unsigned char axis);
int dmp_set_tap_count(unsigned char min_taps);
int dmp_set_tap_time(unsigned short time);
int dmp_set_tap_time_multi(unsigned short time);
int dmp_set_shake_reject_thresh(long sf, unsigned short thresh);
int dmp_set_shake_reject_time(unsigned short time);
int dmp_set_shake_reject_timeout(unsigned short time);

/* Android orientation functions. */
int dmp_register_android_orient_cb(void (*func)(unsigned char));

/* LP quaternion functions. */
int dmp_enable_lp_quat(unsigned char enable);
int dmp_enable_6x_lp_quat(unsigned char enable);

/* Pedometer functions. */
int dmp_get_pedometer_step_count(unsigned long *count);
int dmp_set_pedometer_step_count(unsigned long count);
int dmp_get_pedometer_walk_time(unsigned long *time);
int dmp_set_pedometer_walk_time(unsigned long time);

/* DMP gyro calibration functions. */
int dmp_enable_gyro_cal(unsigned char enable);

/* Read function. This function should be called whenever the MPU interrupt is
 * detected.
 */
int dmp_read_fifo(short *gyro, short *accel, long *quat,
                  unsigned long *timestamp, short *sensors, unsigned char *more);


/* These defines are copied from dmpDefaultMPU6050.c in the general MPL
 * releases. These defines may change for each DMP image, so be sure to modify
 * these values when switching to a new image.
 */
#define CFG_LP_QUAT             (2712)
#define END_ORIENT_TEMP         (1866)
#define CFG_27                  (2742)
#define CFG_20                  (2224)
#define CFG_23                  (2745)
#define CFG_FIFO_ON_EVENT       (2690)
#define END_PREDICTION_UPDATE   (1761)
#define CGNOTICE_INTR           (2620)
#define X_GRT_Y_TMP             (1358)
#define CFG_DR_INT              (1029)
#define CFG_AUTH                (1035)
#define UPDATE_PROP_ROT         (1835)
#define END_COMPARE_Y_X_TMP2    (1455)
#define SKIP_X_GRT_Y_TMP        (1359)
#define SKIP_END_COMPARE        (1435)
#define FCFG_3                  (1088)
#define FCFG_2                  (1066)
#define FCFG_1                  (1062)
#define END_COMPARE_Y_X_TMP3    (1434)
#define FCFG_7                  (1073)
#define FCFG_6                  (1106)
#define FLAT_STATE_END          (1713)
#define SWING_END_4             (1616)
#define SWING_END_2             (1565)
#define SWING_END_3             (1587)
#define SWING_END_1             (1550)
#define CFG_8                   (2718)
#define CFG_15                  (2727)
#define CFG_16                  (2746)
#define CFG_EXT_GYRO_BIAS       (1189)
#define END_COMPARE_Y_X_TMP     (1407)
#define DO_NOT_UPDATE_PROP_ROT  (1839)
#define CFG_7                   (1205)
#define FLAT_STATE_END_TEMP     (1683)
#define END_COMPARE_Y_X         (1484)
#define SKIP_SWING_END_1        (1551)
#define SKIP_SWING_END_3        (1588)
#define SKIP_SWING_END_2        (1566)
#define TILTG75_START           (1672)
#define CFG_6                   (2753)
#define TILTL75_END             (1669)
#define END_ORIENT              (1884)
#define CFG_FLICK_IN            (2573)
#define TILTL75_START           (1643)
#define CFG_MOTION_BIAS         (1208)
#define X_GRT_Y                 (1408)
#define TEMPLABEL               (2324)
#define CFG_ANDROID_ORIENT_INT  (1853)
#define CFG_GYRO_RAW_DATA       (2722)
#define X_GRT_Y_TMP2            (1379)

#define D_0_22                  (22+512)
#define D_0_24                  (24+512)

#define D_0_36                  (36)
#define D_0_52                  (52)
#define D_0_96                  (96)
#define D_0_104                 (104)
#define D_0_108                 (108)
#define D_0_163                 (163)
#define D_0_188                 (188)
#define D_0_192                 (192)
#define D_0_224                 (224)
#define D_0_228                 (228)
#define D_0_232                 (232)
#define D_0_236                 (236)

#define D_1_2                   (256 + 2)
#define D_1_4                   (256 + 4)
#define D_1_8                   (256 + 8)
#define D_1_10                  (256 + 10)
#define D_1_24                  (256 + 24)
#define D_1_28                  (256 + 28)
#define D_1_36                  (256 + 36)
#define D_1_40                  (256 + 40)
#define D_1_44                  (256 + 44)
#define D_1_72                  (256 + 72)
#define D_1_74                  (256 + 74)
#define D_1_79                  (256 + 79)
#define D_1_88                  (256 + 88)
#define D_1_90                  (256 + 90)
#define D_1_92                  (256 + 92)
#define D_1_96                  (256 + 96)
#define D_1_98                  (256 + 98)
#define D_1_106                 (256 + 106)
#define D_1_108                 (256 + 108)
#define D_1_112                 (256 + 112)
#define D_1_128                 (256 + 144)
#define D_1_152                 (256 + 12)
#define D_1_160                 (256 + 160)
#define D_1_176                 (256 + 176)
#define D_1_178                 (256 + 178)
#define D_1_218                 (256 + 218)
#define D_1_232                 (256 + 232)
#define D_1_236                 (256 + 236)
#define D_1_240                 (256 + 240)
#define D_1_244                 (256 + 244)
#define D_1_250                 (256 + 250)
#define D_1_252                 (256 + 252)
#define D_2_12                  (512 + 12)
#define D_2_96                  (512 + 96)
#define D_2_108                 (512 + 108)
#define D_2_208                 (512 + 208)
#define D_2_224                 (512 + 224)
#define D_2_236                 (512 + 236)
#define D_2_244                 (512 + 244)
#define D_2_248                 (512 + 248)
#define D_2_252                 (512 + 252)

#define CPASS_BIAS_X            (35 * 16 + 4)
#define CPASS_BIAS_Y            (35 * 16 + 8)
#define CPASS_BIAS_Z            (35 * 16 + 12)
#define CPASS_MTX_00            (36 * 16)
#define CPASS_MTX_01            (36 * 16 + 4)
#define CPASS_MTX_02            (36 * 16 + 8)
#define CPASS_MTX_10            (36 * 16 + 12)
#define CPASS_MTX_11            (37 * 16)
#define CPASS_MTX_12            (37 * 16 + 4)
#define CPASS_MTX_20            (37 * 16 + 8)
#define CPASS_MTX_21            (37 * 16 + 12)
#define CPASS_MTX_22            (43 * 16 + 12)
#define D_EXT_GYRO_BIAS_X       (61 * 16)
#define D_EXT_GYRO_BIAS_Y       (61 * 16) + 4
#define D_EXT_GYRO_BIAS_Z       (61 * 16) + 8
#define D_ACT0                  (40 * 16)
#define D_ACSX                  (40 * 16 + 4)
#define D_ACSY                  (40 * 16 + 8)
#define D_ACSZ                  (40 * 16 + 12)

#define FLICK_MSG               (45 * 16 + 4)
#define FLICK_COUNTER           (45 * 16 + 8)
#define FLICK_LOWER             (45 * 16 + 12)
#define FLICK_UPPER             (46 * 16 + 12)

#define D_AUTH_OUT              (992)
#define D_AUTH_IN               (996)
#define D_AUTH_A                (1000)
#define D_AUTH_B                (1004)

#define D_PEDSTD_BP_B           (768 + 0x1C)
#define D_PEDSTD_HP_A           (768 + 0x78)
#define D_PEDSTD_HP_B           (768 + 0x7C)
#define D_PEDSTD_BP_A4          (768 + 0x40)
#define D_PEDSTD_BP_A3          (768 + 0x44)
#define D_PEDSTD_BP_A2          (768 + 0x48)
#define D_PEDSTD_BP_A1          (768 + 0x4C)
#define D_PEDSTD_INT_THRSH      (768 + 0x68)
#define D_PEDSTD_CLIP           (768 + 0x6C)
#define D_PEDSTD_SB             (768 + 0x28)
#define D_PEDSTD_SB_TIME        (768 + 0x2C)
#define D_PEDSTD_PEAKTHRSH      (768 + 0x98)
#define D_PEDSTD_TIML           (768 + 0x2A)
#define D_PEDSTD_TIMH           (768 + 0x2E)
#define D_PEDSTD_PEAK           (768 + 0X94)
#define D_PEDSTD_STEPCTR        (768 + 0x60)
#define D_PEDSTD_TIMECTR        (964)
#define D_PEDSTD_DECI           (768 + 0xA0)

#define D_HOST_NO_MOT           (976)
#define D_ACCEL_BIAS            (660)

#define D_ORIENT_GAP            (76)

#define D_TILT0_H               (48)
#define D_TILT0_L               (50)
#define D_TILT1_H               (52)
#define D_TILT1_L               (54)
#define D_TILT2_H               (56)
#define D_TILT2_L               (58)
#define D_TILT3_H               (60)
#define D_TILT3_L               (62)

#define DMP_CODE_SIZE           (3062)

/* END OF SECTION COPIED FROM dmpDefaultMPU6050.c */

#define INT_SRC_TAP             (0x01)
#define INT_SRC_ANDROID_ORIENT  (0x08)

#define DMP_FEATURE_SEND_ANY_GYRO   (DMP_FEATURE_SEND_RAW_GYRO | \
                                     DMP_FEATURE_SEND_CAL_GYRO)

#define DMP_SAMPLE_RATE     (200)
#define GYRO_SF             (46850825LL * 200 / DMP_SAMPLE_RATE)

#define FIFO_CORRUPTION_CHECK
#ifdef FIFO_CORRUPTION_CHECK
#define QUAT_ERROR_THRESH       (1L<<24)
#define QUAT_MAG_SQ_NORMALIZED  (1L<<28)
#define QUAT_MAG_SQ_MIN         (QUAT_MAG_SQ_NORMALIZED - QUAT_ERROR_THRESH)
#define QUAT_MAG_SQ_MAX         (QUAT_MAG_SQ_NORMALIZED + QUAT_ERROR_THRESH)
#endif


//MPU输出速度
#define DEFAULT_MPU_HZ  (100)    //100Hz

#define INV_X_GYRO      (0x40)
#define INV_Y_GYRO      (0x20)
#define INV_Z_GYRO      (0x10)
#define INV_XYZ_GYRO    (INV_X_GYRO | INV_Y_GYRO | INV_Z_GYRO)
#define INV_XYZ_ACCEL   (0x08)
#define INV_XYZ_COMPASS (0x01)

struct int_param_s {
    void (*cb)(void);
    unsigned short pin;
    unsigned char lp_exit;
    unsigned char active_low;
//    unsigned long pin;
//    void (*cb)(volatile void*);
//    void *arg;
};

#define MPU_INT_STATUS_DATA_READY       (0x0001)
#define MPU_INT_STATUS_DMP              (0x0002)
#define MPU_INT_STATUS_PLL_READY        (0x0004)
#define MPU_INT_STATUS_I2C_MST          (0x0008)
#define MPU_INT_STATUS_FIFO_OVERFLOW    (0x0010)
#define MPU_INT_STATUS_ZMOT             (0x0020)
#define MPU_INT_STATUS_MOT              (0x0040)
#define MPU_INT_STATUS_FREE_FALL        (0x0080)
#define MPU_INT_STATUS_DMP_0            (0x0100)
#define MPU_INT_STATUS_DMP_1            (0x0200)
#define MPU_INT_STATUS_DMP_2            (0x0400)
#define MPU_INT_STATUS_DMP_3            (0x0800)
#define MPU_INT_STATUS_DMP_4            (0x1000)
#define MPU_INT_STATUS_DMP_5            (0x2000)

/* Set up APIs */
int mpu_init(void);
int mpu_init_slave(void);
int mpu_set_bypass(unsigned char bypass_on);

/* Configuration APIs */
int mpu_lp_accel_mode(unsigned char rate);
int mpu_lp_motion_interrupt(unsigned short thresh, unsigned char time,
                            unsigned char lpa_freq);
int mpu_set_int_level(unsigned char active_low);
int mpu_set_int_latched(unsigned char enable);

int mpu_set_dmp_state(unsigned char enable);
int mpu_get_dmp_state(unsigned char *enabled);

int mpu_get_lpf(unsigned short *lpf);
int mpu_set_lpf(unsigned short lpf);

int mpu_get_gyro_fsr(unsigned short *fsr);
int mpu_set_gyro_fsr(unsigned short fsr);

int mpu_get_accel_fsr(unsigned char *fsr);
int mpu_set_accel_fsr(unsigned char fsr);

int mpu_get_compass_fsr(unsigned short *fsr);

int mpu_get_gyro_sens(float *sens);
int mpu_get_accel_sens(unsigned short *sens);

int mpu_get_sample_rate(unsigned short *rate);
int mpu_set_sample_rate(unsigned short rate);
int mpu_get_compass_sample_rate(unsigned short *rate);
int mpu_set_compass_sample_rate(unsigned short rate);

int mpu_get_fifo_config(unsigned char *sensors);
int mpu_configure_fifo(unsigned char sensors);

int mpu_get_power_state(unsigned char *power_on);
int mpu_set_sensors(unsigned char sensors);

int mpu_set_accel_bias(const long *accel_bias);

/* Data getter/setter APIs */
int mpu_get_gyro_reg(short *data, unsigned long *timestamp);
int mpu_get_accel_reg(short *data, unsigned long *timestamp);
int mpu_get_compass_reg(short *data, unsigned long *timestamp);
int mpu_get_temperature(long *data, unsigned long *timestamp);

int mpu_get_int_status(short *status);
int mpu_read_fifo(short *gyro, short *accel, unsigned long *timestamp,
                  unsigned char *sensors, unsigned char *more);
int mpu_read_fifo_stream(unsigned short length, unsigned char *data,
                         unsigned char *more);
int mpu_reset_fifo(void);

int mpu_write_mem(unsigned short mem_addr, unsigned short length,
                  unsigned char *data);
int mpu_read_mem(unsigned short mem_addr, unsigned short length,
                 unsigned char *data);
int mpu_load_firmware(unsigned short length, const unsigned char *firmware,
                      unsigned short start_addr, unsigned short sample_rate);

int mpu_reg_dump(void);
int mpu_read_reg(unsigned char reg, unsigned char *data);
int mpu_run_self_test(long *gyro, long *accel);
int mpu_register_tap_cb(void (*func)(unsigned char, unsigned char));


void mget_ms(unsigned long *time);
unsigned short inv_row_2_scale(const signed char *row);
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
uint8_t run_self_test(void);
uint8_t mpu_dmp_init(void);
uint8_t mpu_dmp_get_data(float *pitch,float *roll,float *yaw);

#ifdef __cplusplus
}
#endif


#endif
