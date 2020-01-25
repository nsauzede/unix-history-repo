/*-
 * Copyright (c) 2013, 2014 Andrew Turner
 * Copyright (c) 2015 The FreeBSD Foundation
 * All rights reserved.
 *
 * This software was developed by Andrew Turner under
 * sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef _MACHINE_ARMREG_H_
#define	_MACHINE_ARMREG_H_

#define	INSN_SIZE		4

#define	MRS_MASK			0xfff00000
#define	MRS_VALUE			0xd5300000
#define	MRS_SPECIAL(insn)		((insn) & 0x000fffe0)
#define	MRS_REGISTER(insn)		((insn) & 0x0000001f)
#define	 MRS_Op0_SHIFT			19
#define	 MRS_Op0_MASK			0x00080000
#define	 MRS_Op1_SHIFT			16
#define	 MRS_Op1_MASK			0x00070000
#define	 MRS_CRn_SHIFT			12
#define	 MRS_CRn_MASK			0x0000f000
#define	 MRS_CRm_SHIFT			8
#define	 MRS_CRm_MASK			0x00000f00
#define	 MRS_Op2_SHIFT			5
#define	 MRS_Op2_MASK			0x000000e0
#define	 MRS_Rt_SHIFT			0
#define	 MRS_Rt_MASK			0x0000001f
#define	MRS_REG(op0, op1, crn, crm, op2)				\
    (((op0) << MRS_Op0_SHIFT) | ((op1) << MRS_Op1_SHIFT) |		\
     ((crn) << MRS_CRn_SHIFT) | ((crm) << MRS_CRm_SHIFT) |		\
     ((op2) << MRS_Op2_SHIFT))

#define	READ_SPECIALREG(reg)						\
({	uint64_t _val;							\
	__asm __volatile("mrs	%0, " __STRING(reg) : "=&r" (_val));	\
	_val;								\
})
#define	WRITE_SPECIALREG(reg, _val)					\
	__asm __volatile("msr	" __STRING(reg) ", %0" : : "r"((uint64_t)_val))

#define	UL(x)	UINT64_C(x)

/* CNTHCTL_EL2 - Counter-timer Hypervisor Control register */
#define	CNTHCTL_EVNTI_MASK	(0xf << 4) /* Bit to trigger event stream */
#define	CNTHCTL_EVNTDIR		(1 << 3) /* Control transition trigger bit */
#define	CNTHCTL_EVNTEN		(1 << 2) /* Enable event stream */
#define	CNTHCTL_EL1PCEN		(1 << 1) /* Allow EL0/1 physical timer access */
#define	CNTHCTL_EL1PCTEN	(1 << 0) /*Allow EL0/1 physical counter access*/

/* CPACR_EL1 */
#define	CPACR_FPEN_MASK		(0x3 << 20)
#define	 CPACR_FPEN_TRAP_ALL1	(0x0 << 20) /* Traps from EL0 and EL1 */
#define	 CPACR_FPEN_TRAP_EL0	(0x1 << 20) /* Traps from EL0 */
#define	 CPACR_FPEN_TRAP_ALL2	(0x2 << 20) /* Traps from EL0 and EL1 */
#define	 CPACR_FPEN_TRAP_NONE	(0x3 << 20) /* No traps */
#define	CPACR_TTA		(0x1 << 28)

/* CTR_EL0 - Cache Type Register */
#define	CTR_DLINE_SHIFT		16
#define	CTR_DLINE_MASK		(0xf << CTR_DLINE_SHIFT)
#define	CTR_DLINE_SIZE(reg)	(((reg) & CTR_DLINE_MASK) >> CTR_DLINE_SHIFT)
#define	CTR_ILINE_SHIFT		0
#define	CTR_ILINE_MASK		(0xf << CTR_ILINE_SHIFT)
#define	CTR_ILINE_SIZE(reg)	(((reg) & CTR_ILINE_MASK) >> CTR_ILINE_SHIFT)

/* DAIF - Interrupt Mask Bits */
#define	DAIF_D_MASKED		(1 << 9)
#define	DAIF_A_MASKED		(1 << 8)
#define	DAIF_I_MASKED		(1 << 7)
#define	DAIF_F_MASKED		(1 << 6)

/* DCZID_EL0 - Data Cache Zero ID register */
#define DCZID_DZP		(1 << 4) /* DC ZVA prohibited if non-0 */
#define DCZID_BS_SHIFT		0
#define DCZID_BS_MASK		(0xf << DCZID_BS_SHIFT)
#define	DCZID_BS_SIZE(reg)	(((reg) & DCZID_BS_MASK) >> DCZID_BS_SHIFT)

/* ESR_ELx */
#define	ESR_ELx_ISS_MASK	0x00ffffff
#define	 ISS_INSN_FnV		(0x01 << 10)
#define	 ISS_INSN_EA		(0x01 << 9)
#define	 ISS_INSN_S1PTW		(0x01 << 7)
#define	 ISS_INSN_IFSC_MASK	(0x1f << 0)
#define	 ISS_DATA_ISV		(0x01 << 24)
#define	 ISS_DATA_SAS_MASK	(0x03 << 22)
#define	 ISS_DATA_SSE		(0x01 << 21)
#define	 ISS_DATA_SRT_MASK	(0x1f << 16)
#define	 ISS_DATA_SF		(0x01 << 15)
#define	 ISS_DATA_AR		(0x01 << 14)
#define	 ISS_DATA_FnV		(0x01 << 10)
#define	 ISS_DATA_EA		(0x01 << 9)
#define	 ISS_DATA_CM		(0x01 << 8)
#define	 ISS_DATA_S1PTW		(0x01 << 7)
#define	 ISS_DATA_WnR		(0x01 << 6)
#define	 ISS_DATA_DFSC_MASK	(0x3f << 0)
#define	 ISS_DATA_DFSC_ASF_L0	(0x00 << 0)
#define	 ISS_DATA_DFSC_ASF_L1	(0x01 << 0)
#define	 ISS_DATA_DFSC_ASF_L2	(0x02 << 0)
#define	 ISS_DATA_DFSC_ASF_L3	(0x03 << 0)
#define	 ISS_DATA_DFSC_TF_L0	(0x04 << 0)
#define	 ISS_DATA_DFSC_TF_L1	(0x05 << 0)
#define	 ISS_DATA_DFSC_TF_L2	(0x06 << 0)
#define	 ISS_DATA_DFSC_TF_L3	(0x07 << 0)
#define	 ISS_DATA_DFSC_AFF_L1	(0x09 << 0)
#define	 ISS_DATA_DFSC_AFF_L2	(0x0a << 0)
#define	 ISS_DATA_DFSC_AFF_L3	(0x0b << 0)
#define	 ISS_DATA_DFSC_PF_L1	(0x0d << 0)
#define	 ISS_DATA_DFSC_PF_L2	(0x0e << 0)
#define	 ISS_DATA_DFSC_PF_L3	(0x0f << 0)
#define	 ISS_DATA_DFSC_EXT	(0x10 << 0)
#define	 ISS_DATA_DFSC_EXT_L0	(0x14 << 0)
#define	 ISS_DATA_DFSC_EXT_L1	(0x15 << 0)
#define	 ISS_DATA_DFSC_EXT_L2	(0x16 << 0)
#define	 ISS_DATA_DFSC_EXT_L3	(0x17 << 0)
#define	 ISS_DATA_DFSC_ECC	(0x18 << 0)
#define	 ISS_DATA_DFSC_ECC_L0	(0x1c << 0)
#define	 ISS_DATA_DFSC_ECC_L1	(0x1d << 0)
#define	 ISS_DATA_DFSC_ECC_L2	(0x1e << 0)
#define	 ISS_DATA_DFSC_ECC_L3	(0x1f << 0)
#define	 ISS_DATA_DFSC_ALIGN	(0x21 << 0)
#define	 ISS_DATA_DFSC_TLB_CONFLICT (0x30 << 0)
#define	ESR_ELx_IL		(0x01 << 25)
#define	ESR_ELx_EC_SHIFT	26
#define	ESR_ELx_EC_MASK		(0x3f << 26)
#define	ESR_ELx_EXCEPTION(esr)	(((esr) & ESR_ELx_EC_MASK) >> ESR_ELx_EC_SHIFT)
#define	 EXCP_UNKNOWN		0x00	/* Unkwn exception */
#define	 EXCP_FP_SIMD		0x07	/* VFP/SIMD trap */
#define	 EXCP_ILL_STATE		0x0e	/* Illegal execution state */
#define	 EXCP_SVC32		0x11	/* SVC trap for AArch32 */
#define	 EXCP_SVC64		0x15	/* SVC trap for AArch64 */
#define	 EXCP_MSR		0x18	/* MSR/MRS trap */
#define	 EXCP_INSN_ABORT_L	0x20	/* Instruction abort, from lower EL */
#define	 EXCP_INSN_ABORT	0x21	/* Instruction abort, from same EL */ 
#define	 EXCP_PC_ALIGN		0x22	/* PC alignment fault */
#define	 EXCP_DATA_ABORT_L	0x24	/* Data abort, from lower EL */
#define	 EXCP_DATA_ABORT	0x25	/* Data abort, from same EL */ 
#define	 EXCP_SP_ALIGN		0x26	/* SP slignment fault */
#define	 EXCP_TRAP_FP		0x2c	/* Trapped FP exception */
#define	 EXCP_SERROR		0x2f	/* SError interrupt */
#define	 EXCP_BRKPT_EL0		0x30	/* Hardware breakpoint, from same EL */
#define	 EXCP_SOFTSTP_EL0	0x32	/* Software Step, from lower EL */
#define	 EXCP_SOFTSTP_EL1	0x33	/* Software Step, from same EL */
#define	 EXCP_WATCHPT_EL1	0x35	/* Watchpoint, from same EL */
#define	 EXCP_BRK		0x3c	/* Breakpoint */

/* ICC_CTLR_EL1 */
#define	ICC_CTLR_EL1_EOIMODE	(1U << 1)

/* ICC_IAR1_EL1 */
#define	ICC_IAR1_EL1_SPUR	(0x03ff)

/* ICC_IGRPEN0_EL1 */
#define	ICC_IGRPEN0_EL1_EN	(1U << 0)

/* ICC_PMR_EL1 */
#define	ICC_PMR_EL1_PRIO_MASK	(0xFFUL)

/* ICC_SGI1R_EL1 */
#define	ICC_SGI1R_EL1_TL_MASK		0xffffUL
#define	ICC_SGI1R_EL1_AFF1_SHIFT	16
#define	ICC_SGI1R_EL1_SGIID_SHIFT	24
#define	ICC_SGI1R_EL1_AFF2_SHIFT	32
#define	ICC_SGI1R_EL1_AFF3_SHIFT	48
#define	ICC_SGI1R_EL1_SGIID_MASK	0xfUL
#define	ICC_SGI1R_EL1_IRM		(0x1UL << 40)

/* ICC_SRE_EL1 */
#define	ICC_SRE_EL1_SRE		(1U << 0)

/* ICC_SRE_EL2 */
#define	ICC_SRE_EL2_SRE		(1U << 0)
#define	ICC_SRE_EL2_EN		(1U << 3)

/* ID_AA64DFR0_EL1 */
#define	ID_AA64DFR0_EL1			MRS_REG(3, 0, 0, 5, 0)
#define	ID_AA64DFR0_DebugVer_SHIFT	0
#define	ID_AA64DFR0_DebugVer_MASK	(UL(0xf) << ID_AA64DFR0_DebugVer_SHIFT)
#define	ID_AA64DFR0_DebugVer_VAL(x)	((x) & ID_AA64DFR0_DebugVer_MASK)
#define	 ID_AA64DFR0_DebugVer_8		(UL(0x6) << ID_AA64DFR0_DebugVer_SHIFT)
#define	 ID_AA64DFR0_DebugVer_8_VHE	(UL(0x7) << ID_AA64DFR0_DebugVer_SHIFT)
#define	 ID_AA64DFR0_DebugVer_8_2	(UL(0x8) << ID_AA64DFR0_DebugVer_SHIFT)
#define	ID_AA64DFR0_TraceVer_SHIFT	4
#define	ID_AA64DFR0_TraceVer_MASK	(UL(0xf) << ID_AA64DFR0_TraceVer_SHIFT)
#define	ID_AA64DFR0_TraceVer_VAL(x)	((x) & ID_AA64DFR0_TraceVer_MASK)
#define	 ID_AA64DFR0_TraceVer_NONE	(UL(0x0) << ID_AA64DFR0_TraceVer_SHIFT)
#define	 ID_AA64DFR0_TraceVer_IMPL	(UL(0x1) << ID_AA64DFR0_TraceVer_SHIFT)
#define	ID_AA64DFR0_PMUVer_SHIFT	8
#define	ID_AA64DFR0_PMUVer_MASK		(UL(0xf) << ID_AA64DFR0_PMUVer_SHIFT)
#define	ID_AA64DFR0_PMUVer_VAL(x)	((x) & ID_AA64DFR0_PMUVer_MASK)
#define	 ID_AA64DFR0_PMUVer_NONE	(UL(0x0) << ID_AA64DFR0_PMUVer_SHIFT)
#define	 ID_AA64DFR0_PMUVer_3		(UL(0x1) << ID_AA64DFR0_PMUVer_SHIFT)
#define	 ID_AA64DFR0_PMUVer_3_1		(UL(0x4) << ID_AA64DFR0_PMUVer_SHIFT)
#define	 ID_AA64DFR0_PMUVer_IMPL	(UL(0xf) << ID_AA64DFR0_PMUVer_SHIFT)
#define	ID_AA64DFR0_BRPs_SHIFT		12
#define	ID_AA64DFR0_BRPs_MASK		(UL(0xf) << ID_AA64DFR0_BRPs_SHIFT)
#define	ID_AA64DFR0_BRPs_VAL(x)	\
    ((((x) >> ID_AA64DFR0_BRPs_SHIFT) & 0xf) + 1)
#define	ID_AA64DFR0_WRPs_SHIFT		20
#define	ID_AA64DFR0_WRPs_MASK		(UL(0xf) << ID_AA64DFR0_WRPs_SHIFT)
#define	ID_AA64DFR0_WRPs_VAL(x)	\
    ((((x) >> ID_AA64DFR0_WRPs_SHIFT) & 0xf) + 1)
#define	ID_AA64DFR0_CTX_CMPs_SHIFT	28
#define	ID_AA64DFR0_CTX_CMPs_MASK	(UL(0xf) << ID_AA64DFR0_CTX_CMPs_SHIFT)
#define	ID_AA64DFR0_CTX_CMPs_VAL(x)	\
    ((((x) >> ID_AA64DFR0_CTX_CMPs_SHIFT) & 0xf) + 1)
#define	ID_AA64DFR0_PMSVer_SHIFT	32
#define	ID_AA64DFR0_PMSVer_MASK		(UL(0xf) << ID_AA64DFR0_PMSVer_SHIFT)
#define	ID_AA64DFR0_PMSVer_VAL(x)	((x) & ID_AA64DFR0_PMSVer_MASK)
#define	 ID_AA64DFR0_PMSVer_NONE	(UL(0x0) << ID_AA64DFR0_PMSVer_SHIFT)
#define	 ID_AA64DFR0_PMSVer_V1		(UL(0x1) << ID_AA64DFR0_PMSVer_SHIFT)

/* ID_AA64ISAR0_EL1 */
#define	ID_AA64ISAR0_EL1		MRS_REG(3, 0, 0, 6, 0)
#define	ID_AA64ISAR0_AES_SHIFT		4
#define	ID_AA64ISAR0_AES_MASK		(UL(0xf) << ID_AA64ISAR0_AES_SHIFT)
#define	ID_AA64ISAR0_AES_VAL(x)		((x) & ID_AA64ISAR0_AES_MASK)
#define	 ID_AA64ISAR0_AES_NONE		(UL(0x0) << ID_AA64ISAR0_AES_SHIFT)
#define	 ID_AA64ISAR0_AES_BASE		(UL(0x1) << ID_AA64ISAR0_AES_SHIFT)
#define	 ID_AA64ISAR0_AES_PMULL		(UL(0x2) << ID_AA64ISAR0_AES_SHIFT)
#define	ID_AA64ISAR0_SHA1_SHIFT		8
#define	ID_AA64ISAR0_SHA1_MASK		(UL(0xf) << ID_AA64ISAR0_SHA1_SHIFT)
#define	ID_AA64ISAR0_SHA1_VAL(x)	((x) & ID_AA64ISAR0_SHA1_MASK)
#define	 ID_AA64ISAR0_SHA1_NONE		(UL(0x0) << ID_AA64ISAR0_SHA1_SHIFT)
#define	 ID_AA64ISAR0_SHA1_BASE		(UL(0x1) << ID_AA64ISAR0_SHA1_SHIFT)
#define	ID_AA64ISAR0_SHA2_SHIFT		12
#define	ID_AA64ISAR0_SHA2_MASK		(UL(0xf) << ID_AA64ISAR0_SHA2_SHIFT)
#define	ID_AA64ISAR0_SHA2_VAL(x)	((x) & ID_AA64ISAR0_SHA2_MASK)
#define	 ID_AA64ISAR0_SHA2_NONE		(UL(0x0) << ID_AA64ISAR0_SHA2_SHIFT)
#define	 ID_AA64ISAR0_SHA2_BASE		(UL(0x1) << ID_AA64ISAR0_SHA2_SHIFT)
#define	 ID_AA64ISAR0_SHA2_512		(UL(0x2) << ID_AA64ISAR0_SHA2_SHIFT)
#define	ID_AA64ISAR0_CRC32_SHIFT	16
#define	ID_AA64ISAR0_CRC32_MASK		(UL(0xf) << ID_AA64ISAR0_CRC32_SHIFT)
#define	ID_AA64ISAR0_CRC32_VAL(x)	((x) & ID_AA64ISAR0_CRC32_MASK)
#define	 ID_AA64ISAR0_CRC32_NONE	(UL(0x0) << ID_AA64ISAR0_CRC32_SHIFT)
#define	 ID_AA64ISAR0_CRC32_BASE	(UL(0x1) << ID_AA64ISAR0_CRC32_SHIFT)
#define	ID_AA64ISAR0_Atomic_SHIFT	20
#define	ID_AA64ISAR0_Atomic_MASK	(UL(0xf) << ID_AA64ISAR0_Atomic_SHIFT)
#define	ID_AA64ISAR0_Atomic_VAL(x)	((x) & ID_AA64ISAR0_Atomic_MASK)
#define	 ID_AA64ISAR0_Atomic_NONE	(UL(0x0) << ID_AA64ISAR0_Atomic_SHIFT)
#define	 ID_AA64ISAR0_Atomic_IMPL	(UL(0x2) << ID_AA64ISAR0_Atomic_SHIFT)
#define	ID_AA64ISAR0_RDM_SHIFT		28
#define	ID_AA64ISAR0_RDM_MASK		(UL(0xf) << ID_AA64ISAR0_RDM_SHIFT)
#define	ID_AA64ISAR0_RDM_VAL(x)		((x) & ID_AA64ISAR0_RDM_MASK)
#define	 ID_AA64ISAR0_RDM_NONE		(UL(0x0) << ID_AA64ISAR0_RDM_SHIFT)
#define	 ID_AA64ISAR0_RDM_IMPL		(UL(0x1) << ID_AA64ISAR0_RDM_SHIFT)
#define	ID_AA64ISAR0_SHA3_SHIFT		32
#define	ID_AA64ISAR0_SHA3_MASK		(UL(0xf) << ID_AA64ISAR0_SHA3_SHIFT)
#define	ID_AA64ISAR0_SHA3_VAL(x)	((x) & ID_AA64ISAR0_SHA3_MASK)
#define	 ID_AA64ISAR0_SHA3_NONE		(UL(0x0) << ID_AA64ISAR0_SHA3_SHIFT)
#define	 ID_AA64ISAR0_SHA3_IMPL		(UL(0x1) << ID_AA64ISAR0_SHA3_SHIFT)
#define	ID_AA64ISAR0_SM3_SHIFT		36
#define	ID_AA64ISAR0_SM3_MASK		(UL(0xf) << ID_AA64ISAR0_SM3_SHIFT)
#define	ID_AA64ISAR0_SM3_VAL(x)		((x) & ID_AA64ISAR0_SM3_MASK)
#define	 ID_AA64ISAR0_SM3_NONE		(UL(0x0) << ID_AA64ISAR0_SM3_SHIFT)
#define	 ID_AA64ISAR0_SM3_IMPL		(UL(0x1) << ID_AA64ISAR0_SM3_SHIFT)
#define	ID_AA64ISAR0_SM4_SHIFT		40
#define	ID_AA64ISAR0_SM4_MASK		(UL(0xf) << ID_AA64ISAR0_SM4_SHIFT)
#define	ID_AA64ISAR0_SM4_VAL(x)		((x) & ID_AA64ISAR0_SM4_MASK)
#define	 ID_AA64ISAR0_SM4_NONE		(UL(0x0) << ID_AA64ISAR0_SM4_SHIFT)
#define	 ID_AA64ISAR0_SM4_IMPL		(UL(0x1) << ID_AA64ISAR0_SM4_SHIFT)
#define	ID_AA64ISAR0_DP_SHIFT		44
#define	ID_AA64ISAR0_DP_MASK		(UL(0xf) << ID_AA64ISAR0_DP_SHIFT)
#define	ID_AA64ISAR0_DP_VAL(x)		((x) & ID_AA64ISAR0_DP_MASK)
#define	 ID_AA64ISAR0_DP_NONE		(UL(0x0) << ID_AA64ISAR0_DP_SHIFT)
#define	 ID_AA64ISAR0_DP_IMPL		(UL(0x1) << ID_AA64ISAR0_DP_SHIFT)

/* ID_AA64ISAR1_EL1 */
#define	ID_AA64ISAR1_EL1		MRS_REG(3, 0, 0, 6, 1)
#define	ID_AA64ISAR1_DPB_SHIFT		0
#define	ID_AA64ISAR1_DPB_MASK		(UL(0xf) << ID_AA64ISAR1_DPB_SHIFT)
#define	ID_AA64ISAR1_DPB_VAL(x)		((x) & ID_AA64ISAR1_DPB_MASK)
#define	 ID_AA64ISAR1_DPB_NONE		(UL(0x0) << ID_AA64ISAR1_DPB_SHIFT)
#define	 ID_AA64ISAR1_DPB_IMPL		(UL(0x1) << ID_AA64ISAR1_DPB_SHIFT)
#define	ID_AA64ISAR1_APA_SHIFT		4
#define	ID_AA64ISAR1_APA_MASK		(UL(0xf) << ID_AA64ISAR1_APA_SHIFT)
#define	ID_AA64ISAR1_APA_VAL(x)		((x) & ID_AA64ISAR1_APA_MASK)
#define	 ID_AA64ISAR1_APA_NONE		(UL(0x0) << ID_AA64ISAR1_APA_SHIFT)
#define	 ID_AA64ISAR1_APA_IMPL		(UL(0x1) << ID_AA64ISAR1_APA_SHIFT)
#define	ID_AA64ISAR1_API_SHIFT		8
#define	ID_AA64ISAR1_API_MASK		(UL(0xf) << ID_AA64ISAR1_API_SHIFT)
#define	ID_AA64ISAR1_API_VAL(x)		((x) & ID_AA64ISAR1_API_MASK)
#define	 ID_AA64ISAR1_API_NONE		(UL(0x0) << ID_AA64ISAR1_API_SHIFT)
#define	 ID_AA64ISAR1_API_IMPL		(UL(0x1) << ID_AA64ISAR1_API_SHIFT)
#define	ID_AA64ISAR1_JSCVT_SHIFT	12
#define	ID_AA64ISAR1_JSCVT_MASK		(UL(0xf) << ID_AA64ISAR1_JSCVT_SHIFT)
#define	ID_AA64ISAR1_JSCVT_VAL(x)	((x) & ID_AA64ISAR1_JSCVT_MASK)
#define	 ID_AA64ISAR1_JSCVT_NONE	(UL(0x0) << ID_AA64ISAR1_JSCVT_SHIFT)
#define	 ID_AA64ISAR1_JSCVT_IMPL	(UL(0x1) << ID_AA64ISAR1_JSCVT_SHIFT)
#define	ID_AA64ISAR1_FCMA_SHIFT		16
#define	ID_AA64ISAR1_FCMA_MASK		(UL(0xf) << ID_AA64ISAR1_FCMA_SHIFT)
#define	ID_AA64ISAR1_FCMA_VAL(x)	((x) & ID_AA64ISAR1_FCMA_MASK)
#define	 ID_AA64ISAR1_FCMA_NONE		(UL(0x0) << ID_AA64ISAR1_FCMA_SHIFT)
#define	 ID_AA64ISAR1_FCMA_IMPL		(UL(0x1) << ID_AA64ISAR1_FCMA_SHIFT)
#define	ID_AA64ISAR1_LRCPC_SHIFT	20
#define	ID_AA64ISAR1_LRCPC_MASK		(UL(0xf) << ID_AA64ISAR1_LRCPC_SHIFT)
#define	ID_AA64ISAR1_LRCPC_VAL(x)	((x) & ID_AA64ISAR1_LRCPC_MASK)
#define	 ID_AA64ISAR1_LRCPC_NONE	(UL(0x0) << ID_AA64ISAR1_LRCPC_SHIFT)
#define	 ID_AA64ISAR1_LRCPC_IMPL	(UL(0x1) << ID_AA64ISAR1_LRCPC_SHIFT)
#define	ID_AA64ISAR1_GPA_SHIFT		24
#define	ID_AA64ISAR1_GPA_MASK		(UL(0xf) << ID_AA64ISAR1_GPA_SHIFT)
#define	ID_AA64ISAR1_GPA_VAL(x)		((x) & ID_AA64ISAR1_GPA_MASK)
#define	 ID_AA64ISAR1_GPA_NONE		(UL(0x0) << ID_AA64ISAR1_GPA_SHIFT)
#define	 ID_AA64ISAR1_GPA_IMPL		(UL(0x1) << ID_AA64ISAR1_GPA_SHIFT)
#define	ID_AA64ISAR1_GPI_SHIFT		28
#define	ID_AA64ISAR1_GPI_MASK		(UL(0xf) << ID_AA64ISAR1_GPI_SHIFT)
#define	ID_AA64ISAR1_GPI_VAL(x)		((x) & ID_AA64ISAR1_GPI_MASK)
#define	 ID_AA64ISAR1_GPI_NONE		(UL(0x0) << ID_AA64ISAR1_GPI_SHIFT)
#define	 ID_AA64ISAR1_GPI_IMPL		(UL(0x1) << ID_AA64ISAR1_GPI_SHIFT)

/* ID_AA64MMFR0_EL1 */
#define	ID_AA64MMFR0_EL1		MRS_REG(3, 0, 0, 7, 0)
#define	ID_AA64MMFR0_PARange_SHIFT	0
#define	ID_AA64MMFR0_PARange_MASK	(UL(0xf) << ID_AA64MMFR0_PARange_SHIFT)
#define	ID_AA64MMFR0_PARange_VAL(x)	((x) & ID_AA64MMFR0_PARange_MASK)
#define	 ID_AA64MMFR0_PARange_4G	(UL(0x0) << ID_AA64MMFR0_PARange_SHIFT)
#define	 ID_AA64MMFR0_PARange_64G	(UL(0x1) << ID_AA64MMFR0_PARange_SHIFT)
#define	 ID_AA64MMFR0_PARange_1T	(UL(0x2) << ID_AA64MMFR0_PARange_SHIFT)
#define	 ID_AA64MMFR0_PARange_4T	(UL(0x3) << ID_AA64MMFR0_PARange_SHIFT)
#define	 ID_AA64MMFR0_PARange_16T	(UL(0x4) << ID_AA64MMFR0_PARange_SHIFT)
#define	 ID_AA64MMFR0_PARange_256T	(UL(0x5) << ID_AA64MMFR0_PARange_SHIFT)
#define	 ID_AA64MMFR0_PARange_4P	(UL(0x6) << ID_AA64MMFR0_PARange_SHIFT)
#define	ID_AA64MMFR0_ASIDBits_SHIFT	4
#define	ID_AA64MMFR0_ASIDBits_MASK	(UL(0xf) << ID_AA64MMFR0_ASIDBits_SHIFT)
#define	ID_AA64MMFR0_ASIDBits_VAL(x)	((x) & ID_AA64MMFR0_ASIDBits_MASK)
#define	 ID_AA64MMFR0_ASIDBits_8	(UL(0x0) << ID_AA64MMFR0_ASIDBits_SHIFT)
#define	 ID_AA64MMFR0_ASIDBits_16	(UL(0x2) << ID_AA64MMFR0_ASIDBits_SHIFT)
#define	ID_AA64MMFR0_BigEnd_SHIFT	8
#define	ID_AA64MMFR0_BigEnd_MASK	(UL(0xf) << ID_AA64MMFR0_BigEnd_SHIFT)
#define	ID_AA64MMFR0_BigEnd_VAL(x)	((x) & ID_AA64MMFR0_BigEnd_MASK)
#define	 ID_AA64MMFR0_BigEnd_FIXED	(UL(0x0) << ID_AA64MMFR0_BigEnd_SHIFT)
#define	 ID_AA64MMFR0_BigEnd_MIXED	(UL(0x1) << ID_AA64MMFR0_BigEnd_SHIFT)
#define	ID_AA64MMFR0_SNSMem_SHIFT	12
#define	ID_AA64MMFR0_SNSMem_MASK	(UL(0xf) << ID_AA64MMFR0_SNSMem_SHIFT)
#define	ID_AA64MMFR0_SNSMem_VAL(x)	((x) & ID_AA64MMFR0_SNSMem_MASK)
#define	 ID_AA64MMFR0_SNSMem_NONE	(UL(0x0) << ID_AA64MMFR0_SNSMem_SHIFT)
#define	 ID_AA64MMFR0_SNSMem_DISTINCT	(UL(0x1) << ID_AA64MMFR0_SNSMem_SHIFT)
#define	ID_AA64MMFR0_BigEndEL0_SHIFT	16
#define	ID_AA64MMFR0_BigEndEL0_MASK	(UL(0xf) << ID_AA64MMFR0_BigEndEL0_SHIFT)
#define	ID_AA64MMFR0_BigEndEL0_VAL(x)	((x) & ID_AA64MMFR0_BigEndEL0_MASK)
#define	 ID_AA64MMFR0_BigEndEL0_FIXED	(UL(0x0) << ID_AA64MMFR0_BigEndEL0_SHIFT)
#define	 ID_AA64MMFR0_BigEndEL0_MIXED	(UL(0x1) << ID_AA64MMFR0_BigEndEL0_SHIFT)
#define	ID_AA64MMFR0_TGran16_SHIFT	20
#define	ID_AA64MMFR0_TGran16_MASK	(UL(0xf) << ID_AA64MMFR0_TGran16_SHIFT)
#define	ID_AA64MMFR0_TGran16_VAL(x)	((x) & ID_AA64MMFR0_TGran16_MASK)
#define	 ID_AA64MMFR0_TGran16_NONE	(UL(0x0) << ID_AA64MMFR0_TGran16_SHIFT)
#define	 ID_AA64MMFR0_TGran16_IMPL	(UL(0x1) << ID_AA64MMFR0_TGran16_SHIFT)
#define	ID_AA64MMFR0_TGran64_SHIFT	24
#define	ID_AA64MMFR0_TGran64_MASK	(UL(0xf) << ID_AA64MMFR0_TGran64_SHIFT)
#define	ID_AA64MMFR0_TGran64_VAL(x)	((x) & ID_AA64MMFR0_TGran64_MASK)
#define	 ID_AA64MMFR0_TGran64_IMPL	(UL(0x0) << ID_AA64MMFR0_TGran64_SHIFT)
#define	 ID_AA64MMFR0_TGran64_NONE	(UL(0xf) << ID_AA64MMFR0_TGran64_SHIFT)
#define	ID_AA64MMFR0_TGran4_SHIFT	28
#define	ID_AA64MMFR0_TGran4_MASK	(UL(0xf) << ID_AA64MMFR0_TGran4_SHIFT)
#define	ID_AA64MMFR0_TGran4_VAL(x)	((x) & ID_AA64MMFR0_TGran4_MASK)
#define	 ID_AA64MMFR0_TGran4_IMPL	(UL(0x0) << ID_AA64MMFR0_TGran4_SHIFT)
#define	 ID_AA64MMFR0_TGran4_NONE	(UL(0xf) << ID_AA64MMFR0_TGran4_SHIFT)

/* ID_AA64MMFR1_EL1 */
#define	ID_AA64MMFR1_EL1		MRS_REG(3, 0, 0, 7, 1)
#define	ID_AA64MMFR1_HAFDBS_SHIFT	0
#define	ID_AA64MMFR1_HAFDBS_MASK	(UL(0xf) << ID_AA64MMFR1_HAFDBS_SHIFT)
#define	ID_AA64MMFR1_HAFDBS_VAL(x)	((x) & ID_AA64MMFR1_HAFDBS_MASK)
#define	 ID_AA64MMFR1_HAFDBS_NONE	(UL(0x0) << ID_AA64MMFR1_HAFDBS_SHIFT)
#define	 ID_AA64MMFR1_HAFDBS_AF		(UL(0x1) << ID_AA64MMFR1_HAFDBS_SHIFT)
#define	 ID_AA64MMFR1_HAFDBS_AF_DBS	(UL(0x2) << ID_AA64MMFR1_HAFDBS_SHIFT)
#define	ID_AA64MMFR1_VMIDBits_SHIFT	4
#define	ID_AA64MMFR1_VMIDBits_MASK	(UL(0xf) << ID_AA64MMFR1_VMIDBits_SHIFT)
#define	ID_AA64MMFR1_VMIDBits_VAL(x)	((x) & ID_AA64MMFR1_VMIDBits_MASK)
#define	 ID_AA64MMFR1_VMIDBits_8	(UL(0x0) << ID_AA64MMFR1_VMIDBits_SHIFT)
#define	 ID_AA64MMFR1_VMIDBits_16	(UL(0x2) << ID_AA64MMFR1_VMIDBits_SHIFT)
#define	ID_AA64MMFR1_VH_SHIFT		8
#define	ID_AA64MMFR1_VH_MASK		(UL(0xf) << ID_AA64MMFR1_VH_SHIFT)
#define	ID_AA64MMFR1_VH_VAL(x)		((x) & ID_AA64MMFR1_VH_MASK)
#define	 ID_AA64MMFR1_VH_NONE		(UL(0x0) << ID_AA64MMFR1_VH_SHIFT)
#define	 ID_AA64MMFR1_VH_IMPL		(UL(0x1) << ID_AA64MMFR1_VH_SHIFT)
#define	ID_AA64MMFR1_HPDS_SHIFT		12
#define	ID_AA64MMFR1_HPDS_MASK		(UL(0xf) << ID_AA64MMFR1_HPDS_SHIFT)
#define	ID_AA64MMFR1_HPDS_VAL(x)	((x) & ID_AA64MMFR1_HPDS_MASK)
#define	 ID_AA64MMFR1_HPDS_NONE		(UL(0x0) << ID_AA64MMFR1_HPDS_SHIFT)
#define	 ID_AA64MMFR1_HPDS_HPD		(UL(0x1) << ID_AA64MMFR1_HPDS_SHIFT)
#define	 ID_AA64MMFR1_HPDS_TTPBHA	(UL(0x2) << ID_AA64MMFR1_HPDS_SHIFT)
#define	ID_AA64MMFR1_LO_SHIFT		16
#define	ID_AA64MMFR1_LO_MASK		(UL(0xf) << ID_AA64MMFR1_LO_SHIFT)
#define	ID_AA64MMFR1_LO_VAL(x)		((x) & ID_AA64MMFR1_LO_MASK)
#define	 ID_AA64MMFR1_LO_NONE		(UL(0x0) << ID_AA64MMFR1_LO_SHIFT)
#define	 ID_AA64MMFR1_LO_IMPL		(UL(0x1) << ID_AA64MMFR1_LO_SHIFT)
#define	ID_AA64MMFR1_PAN_SHIFT		20
#define	ID_AA64MMFR1_PAN_MASK		(UL(0xf) << ID_AA64MMFR1_PAN_SHIFT)
#define	ID_AA64MMFR1_PAN_VAL(x)		((x) & ID_AA64MMFR1_PAN_MASK)
#define	 ID_AA64MMFR1_PAN_NONE		(UL(0x0) << ID_AA64MMFR1_PAN_SHIFT)
#define	 ID_AA64MMFR1_PAN_IMPL		(UL(0x1) << ID_AA64MMFR1_PAN_SHIFT)
#define	 ID_AA64MMFR1_PAN_ATS1E1	(UL(0x2) << ID_AA64MMFR1_PAN_SHIFT)
#define	ID_AA64MMFR1_SpecSEI_SHIFT	24
#define	ID_AA64MMFR1_SpecSEI_MASK	(UL(0xf) << ID_AA64MMFR1_SpecSEI_SHIFT)
#define	ID_AA64MMFR1_SpecSEI_VAL(x)	((x) & ID_AA64MMFR1_SpecSEI_MASK)
#define	 ID_AA64MMFR1_SpecSEI_NONE	(UL(0x0) << ID_AA64MMFR1_SpecSEI_SHIFT)
#define	 ID_AA64MMFR1_SpecSEI_IMPL	(UL(0x1) << ID_AA64MMFR1_SpecSEI_SHIFT)
#define	ID_AA64MMFR1_XNX_SHIFT		28
#define	ID_AA64MMFR1_XNX_MASK		(UL(0xf) << ID_AA64MMFR1_XNX_SHIFT)
#define	ID_AA64MMFR1_XNX_VAL(x)		((x) & ID_AA64MMFR1_XNX_MASK)
#define	 ID_AA64MMFR1_XNX_NONE		(UL(0x0) << ID_AA64MMFR1_XNX_SHIFT)
#define	 ID_AA64MMFR1_XNX_IMPL		(UL(0x1) << ID_AA64MMFR1_XNX_SHIFT)

/* ID_AA64MMFR2_EL1 */
#define	ID_AA64MMFR2_EL1		MRS_REG(3, 0, 0, 7, 2)
#define	ID_AA64MMFR2_CnP_SHIFT		0
#define	ID_AA64MMFR2_CnP_MASK		(UL(0xf) << ID_AA64MMFR2_CnP_SHIFT)
#define	ID_AA64MMFR2_CnP_VAL(x)		((x) & ID_AA64MMFR2_CnP_MASK)
#define	 ID_AA64MMFR2_CnP_NONE		(UL(0x0) << ID_AA64MMFR2_CnP_SHIFT)
#define	 ID_AA64MMFR2_CnP_IMPL		(UL(0x1) << ID_AA64MMFR2_CnP_SHIFT)
#define	ID_AA64MMFR2_UAO_SHIFT		4
#define	ID_AA64MMFR2_UAO_MASK		(UL(0xf) << ID_AA64MMFR2_UAO_SHIFT)
#define	ID_AA64MMFR2_UAO_VAL(x)		((x) & ID_AA64MMFR2_UAO_MASK)
#define	 ID_AA64MMFR2_UAO_NONE		(UL(0x0) << ID_AA64MMFR2_UAO_SHIFT)
#define	 ID_AA64MMFR2_UAO_IMPL		(UL(0x1) << ID_AA64MMFR2_UAO_SHIFT)
#define	ID_AA64MMFR2_LSM_SHIFT		8
#define	ID_AA64MMFR2_LSM_MASK		(UL(0xf) << ID_AA64MMFR2_LSM_SHIFT)
#define	ID_AA64MMFR2_LSM_VAL(x)		((x) & ID_AA64MMFR2_LSM_MASK)
#define	 ID_AA64MMFR2_LSM_NONE		(UL(0x0) << ID_AA64MMFR2_LSM_SHIFT)
#define	 ID_AA64MMFR2_LSM_IMPL		(UL(0x1) << ID_AA64MMFR2_LSM_SHIFT)
#define	ID_AA64MMFR2_IESB_SHIFT		12
#define	ID_AA64MMFR2_IESB_MASK		(UL(0xf) << ID_AA64MMFR2_IESB_SHIFT)
#define	ID_AA64MMFR2_IESB_VAL(x)	((x) & ID_AA64MMFR2_IESB_MASK)
#define	 ID_AA64MMFR2_IESB_NONE		(UL(0x0) << ID_AA64MMFR2_IESB_SHIFT)
#define	 ID_AA64MMFR2_IESB_IMPL		(UL(0x1) << ID_AA64MMFR2_IESB_SHIFT)
#define	ID_AA64MMFR2_VARange_SHIFT	16
#define	ID_AA64MMFR2_VARange_MASK	(UL(0xf) << ID_AA64MMFR2_VARange_SHIFT)
#define	ID_AA64MMFR2_VARange_VAL(x)	((x) & ID_AA64MMFR2_VARange_MASK)
#define	 ID_AA64MMFR2_VARange_48	(UL(0x0) << ID_AA64MMFR2_VARange_SHIFT)
#define	 ID_AA64MMFR2_VARange_52	(UL(0x1) << ID_AA64MMFR2_VARange_SHIFT)
#define	ID_AA64MMFR2_CCIDX_SHIFT	20
#define	ID_AA64MMFR2_CCIDX_MASK		(UL(0xf) << ID_AA64MMFR2_CCIDX_SHIFT)
#define	ID_AA64MMFR2_CCIDX_VAL(x)	((x) & ID_AA64MMFR2_CCIDX_MASK)
#define	 ID_AA64MMFR2_CCIDX_32		(UL(0x0) << ID_AA64MMFR2_CCIDX_SHIFT)
#define	 ID_AA64MMFR2_CCIDX_64		(UL(0x1) << ID_AA64MMFR2_CCIDX_SHIFT)
#define	ID_AA64MMFR2_NV_SHIFT		24
#define	ID_AA64MMFR2_NV_MASK		(UL(0xf) << ID_AA64MMFR2_NV_SHIFT)
#define	ID_AA64MMFR2_NV_VAL(x)		((x) & ID_AA64MMFR2_NV_MASK)
#define	 ID_AA64MMFR2_NV_NONE		(UL(0x0) << ID_AA64MMFR2_NV_SHIFT)
#define	 ID_AA64MMFR2_NV_IMPL		(UL(0x1) << ID_AA64MMFR2_NV_SHIFT)

/* ID_AA64PFR0_EL1 */
#define	ID_AA64PFR0_EL1			MRS_REG(3, 0, 0, 4, 0)
#define	ID_AA64PFR0_EL0_SHIFT		0
#define	ID_AA64PFR0_EL0_MASK		(UL(0xf) << ID_AA64PFR0_EL0_SHIFT)
#define	ID_AA64PFR0_EL0_VAL(x)		((x) & ID_AA64PFR0_EL0_MASK)
#define	 ID_AA64PFR0_EL0_64		(UL(0x1) << ID_AA64PFR0_EL0_SHIFT)
#define	 ID_AA64PFR0_EL0_64_32		(UL(0x2) << ID_AA64PFR0_EL0_SHIFT)
#define	ID_AA64PFR0_EL1_SHIFT		4
#define	ID_AA64PFR0_EL1_MASK		(UL(0xf) << ID_AA64PFR0_EL1_SHIFT)
#define	ID_AA64PFR0_EL1_VAL(x)		((x) & ID_AA64PFR0_EL1_MASK)
#define	 ID_AA64PFR0_EL1_64		(UL(0x1) << ID_AA64PFR0_EL1_SHIFT)
#define	 ID_AA64PFR0_EL1_64_32		(UL(0x2) << ID_AA64PFR0_EL1_SHIFT)
#define	ID_AA64PFR0_EL2_SHIFT		8
#define	ID_AA64PFR0_EL2_MASK		(UL(0xf) << ID_AA64PFR0_EL2_SHIFT)
#define	ID_AA64PFR0_EL2_VAL(x)		((x) & ID_AA64PFR0_EL2_MASK)
#define	 ID_AA64PFR0_EL2_NONE		(UL(0x0) << ID_AA64PFR0_EL2_SHIFT)
#define	 ID_AA64PFR0_EL2_64		(UL(0x1) << ID_AA64PFR0_EL2_SHIFT)
#define	 ID_AA64PFR0_EL2_64_32		(UL(0x2) << ID_AA64PFR0_EL2_SHIFT)
#define	ID_AA64PFR0_EL3_SHIFT		12
#define	ID_AA64PFR0_EL3_MASK		(UL(0xf) << ID_AA64PFR0_EL3_SHIFT)
#define	ID_AA64PFR0_EL3_VAL(x)		((x) & ID_AA64PFR0_EL3_MASK)
#define	 ID_AA64PFR0_EL3_NONE		(UL(0x0) << ID_AA64PFR0_EL3_SHIFT)
#define	 ID_AA64PFR0_EL3_64		(UL(0x1) << ID_AA64PFR0_EL3_SHIFT)
#define	 ID_AA64PFR0_EL3_64_32		(UL(0x2) << ID_AA64PFR0_EL3_SHIFT)
#define	ID_AA64PFR0_FP_SHIFT		16
#define	ID_AA64PFR0_FP_MASK		(UL(0xf) << ID_AA64PFR0_FP_SHIFT)
#define	ID_AA64PFR0_FP_VAL(x)		((x) & ID_AA64PFR0_FP_MASK)
#define	 ID_AA64PFR0_FP_IMPL		(UL(0x0) << ID_AA64PFR0_FP_SHIFT)
#define	 ID_AA64PFR0_FP_HP		(UL(0x1) << ID_AA64PFR0_FP_SHIFT)
#define	 ID_AA64PFR0_FP_NONE		(UL(0xf) << ID_AA64PFR0_FP_SHIFT)
#define	ID_AA64PFR0_AdvSIMD_SHIFT	20
#define	ID_AA64PFR0_AdvSIMD_MASK	(UL(0xf) << ID_AA64PFR0_AdvSIMD_SHIFT)
#define	ID_AA64PFR0_AdvSIMD_VAL(x)	((x) & ID_AA64PFR0_AdvSIMD_MASK)
#define	 ID_AA64PFR0_AdvSIMD_IMPL	(UL(0x0) << ID_AA64PFR0_AdvSIMD_SHIFT)
#define	 ID_AA64PFR0_AdvSIMD_HP		(UL(0x1) << ID_AA64PFR0_AdvSIMD_SHIFT)
#define	 ID_AA64PFR0_AdvSIMD_NONE	(UL(0xf) << ID_AA64PFR0_AdvSIMD_SHIFT)
#define	ID_AA64PFR0_GIC_BITS		0x4 /* Number of bits in GIC field */
#define	ID_AA64PFR0_GIC_SHIFT		24
#define	ID_AA64PFR0_GIC_MASK		(UL(0xf) << ID_AA64PFR0_GIC_SHIFT)
#define	ID_AA64PFR0_GIC_VAL(x)		((x) & ID_AA64PFR0_GIC_MASK)
#define	 ID_AA64PFR0_GIC_CPUIF_NONE	(UL(0x0) << ID_AA64PFR0_GIC_SHIFT)
#define	 ID_AA64PFR0_GIC_CPUIF_EN	(UL(0x1) << ID_AA64PFR0_GIC_SHIFT)
#define	ID_AA64PFR0_RAS_SHIFT		28
#define	ID_AA64PFR0_RAS_MASK		(UL(0xf) << ID_AA64PFR0_RAS_SHIFT)
#define	ID_AA64PFR0_RAS_VAL(x)		((x) & ID_AA64PFR0_RAS_MASK)
#define	 ID_AA64PFR0_RAS_NONE		(UL(0x0) << ID_AA64PFR0_RAS_SHIFT)
#define	 ID_AA64PFR0_RAS_V1		(UL(0x1) << ID_AA64PFR0_RAS_SHIFT)
#define	ID_AA64PFR0_SVE_SHIFT		32
#define	ID_AA64PFR0_SVE_MASK		(UL(0xf) << ID_AA64PFR0_SVE_SHIFT)
#define	ID_AA64PFR0_SVE_VAL(x)		((x) & ID_AA64PFR0_SVE_MASK)
#define	 ID_AA64PFR0_SVE_NONE		(UL(0x0) << ID_AA64PFR0_SVE_SHIFT)
#define	 ID_AA64PFR0_SVE_IMPL		(UL(0x1) << ID_AA64PFR0_SVE_SHIFT)
#define	ID_AA64PFR0_SEL2_SHIFT		36
#define	ID_AA64PFR0_SEL2_MASK		(UL(0xf) << ID_AA64PFR0_SEL2_SHIFT)
#define	ID_AA64PFR0_SEL2_VAL(x)		((x) & ID_AA64PFR0_SEL2_MASK)
#define	 ID_AA64PFR0_SEL2_NONE		(UL(0x0) << ID_AA64PFR0_SEL2_SHIFT)
#define	 ID_AA64PFR0_SEL2_IMPL		(UL(0x1) << ID_AA64PFR0_SEL2_SHIFT)
#define	ID_AA64PFR0_MPAM_SHIFT		40
#define	ID_AA64PFR0_MPAM_MASK		(UL(0xf) << ID_AA64PFR0_MPAM_SHIFT)
#define	ID_AA64PFR0_MPAM_VAL(x)		((x) & ID_AA64PFR0_MPAM_MASK)
#define	 ID_AA64PFR0_MPAM_NONE		(UL(0x0) << ID_AA64PFR0_MPAM_SHIFT)
#define	 ID_AA64PFR0_MPAM_IMPL		(UL(0x1) << ID_AA64PFR0_MPAM_SHIFT)
#define	ID_AA64PFR0_AMU_SHIFT		44
#define	ID_AA64PFR0_AMU_MASK		(UL(0xf) << ID_AA64PFR0_AMU_SHIFT)
#define	ID_AA64PFR0_AMU_VAL(x)		((x) & ID_AA64PFR0_AMU_MASK)
#define	 ID_AA64PFR0_AMU_NONE		(UL(0x0) << ID_AA64PFR0_AMU_SHIFT)
#define	 ID_AA64PFR0_AMU_V1		(UL(0x1) << ID_AA64PFR0_AMU_SHIFT)
#define	ID_AA64PFR0_DIT_SHIFT		48
#define	ID_AA64PFR0_DIT_MASK		(UL(0xf) << ID_AA64PFR0_DIT_SHIFT)
#define	ID_AA64PFR0_DIT_VAL(x)		((x) & ID_AA64PFR0_DIT_MASK)
#define	 ID_AA64PFR0_DIT_NONE		(UL(0x0) << ID_AA64PFR0_DIT_SHIFT)
#define	 ID_AA64PFR0_DIT_PSTATE		(UL(0x1) << ID_AA64PFR0_DIT_SHIFT)
#define	ID_AA64PFR0_CSV2_SHIFT		56
#define	ID_AA64PFR0_CSV2_MASK		(UL(0xf) << ID_AA64PFR0_CSV2_SHIFT)
#define	ID_AA64PFR0_CSV2_VAL(x)		((x) & ID_AA64PFR0_CSV2_MASK)
#define	 ID_AA64PFR0_CSV2_NONE		(UL(0x0) << ID_AA64PFR0_CSV2_SHIFT)
#define	 ID_AA64PFR0_CSV2_ISOLATED	(UL(0x1) << ID_AA64PFR0_CSV2_SHIFT)
#define	 ID_AA64PFR0_CSV2_SCXTNUM	(UL(0x2) << ID_AA64PFR0_CSV2_SHIFT)
#define	ID_AA64PFR0_CSV3_SHIFT		60
#define	ID_AA64PFR0_CSV3_MASK		(UL(0xf) << ID_AA64PFR0_CSV3_SHIFT)
#define	ID_AA64PFR0_CSV3_VAL(x)		((x) & ID_AA64PFR0_CSV3_MASK)
#define	 ID_AA64PFR0_CSV3_NONE		(UL(0x0) << ID_AA64PFR0_CSV3_SHIFT)
#define	 ID_AA64PFR0_CSV3_ISOLATED	(UL(0x1) << ID_AA64PFR0_CSV3_SHIFT)

/* ID_AA64PFR1_EL1 */
#define	ID_AA64PFR1_EL1			MRS_REG(3, 0, 0, 4, 1)
#define	ID_AA64PFR1_BT_SHIFT		0
#define	ID_AA64PFR1_BT_MASK		(UL(0xf) << ID_AA64PFR1_BT_SHIFT)
#define	ID_AA64PFR1_BT_VAL(x)		((x) & ID_AA64PFR1_BT_MASK)
#define	 ID_AA64PFR1_BT_NONE		(UL(0x0) << ID_AA64PFR1_BT_SHIFT)
#define	 ID_AA64PFR1_BT_IMPL		(UL(0x1) << ID_AA64PFR1_BT_SHIFT)
#define	ID_AA64PFR1_SSBS_SHIFT		4
#define	ID_AA64PFR1_SSBS_MASK		(UL(0xf) << ID_AA64PFR1_SSBS_SHIFT)
#define	ID_AA64PFR1_SSBS_VAL(x)		((x) & ID_AA64PFR1_SSBS_MASK)
#define	 ID_AA64PFR1_SSBS_NONE		(UL(0x0) << ID_AA64PFR1_SSBS_SHIFT)
#define	 ID_AA64PFR1_SSBS_PSTATE	(UL(0x1) << ID_AA64PFR1_SSBS_SHIFT)
#define	 ID_AA64PFR1_SSBS_PSTATE_MSR	(UL(0x2) << ID_AA64PFR1_SSBS_SHIFT)
#define	ID_AA64PFR1_MTE_SHIFT		8
#define	ID_AA64PFR1_MTE_MASK		(UL(0xf) << ID_AA64PFR1_MTE_SHIFT)
#define	ID_AA64PFR1_MTE_VAL(x)		((x) & ID_AA64PFR1_MTE_MASK)
#define	 ID_AA64PFR1_MTE_NONE		(UL(0x0) << ID_AA64PFR1_MTE_SHIFT)
#define	 ID_AA64PFR1_MTE_IMPL_EL0	(UL(0x1) << ID_AA64PFR1_MTE_SHIFT)
#define	 ID_AA64PFR1_MTE_IMPL		(UL(0x2) << ID_AA64PFR1_MTE_SHIFT)
#define	ID_AA64PFR1_RAS_frac_SHIFT	12
#define	ID_AA64PFR1_RAS_frac_MASK	(UL(0xf) << ID_AA64PFR1_RAS_frac_SHIFT)
#define	ID_AA64PFR1_RAS_frac_VAL(x)	((x) & ID_AA64PFR1_RAS_frac_MASK)
#define	 ID_AA64PFR1_RAS_frac_V1	(UL(0x0) << ID_AA64PFR1_RAS_frac_SHIFT)
#define	 ID_AA64PFR1_RAS_frac_V2	(UL(0x1) << ID_AA64PFR1_RAS_frac_SHIFT)

/* MAIR_EL1 - Memory Attribute Indirection Register */
#define	MAIR_ATTR_MASK(idx)	(0xff << ((n)* 8))
#define	MAIR_ATTR(attr, idx) ((attr) << ((idx) * 8))
#define	 MAIR_DEVICE_nGnRnE	0x00
#define	 MAIR_NORMAL_NC		0x44
#define	 MAIR_NORMAL_WT		0xbb
#define	 MAIR_NORMAL_WB		0xff

/* PAR_EL1 - Physical Address Register */
#define	PAR_F_SHIFT		0
#define	PAR_F			(0x1 << PAR_F_SHIFT)
#define	PAR_SUCCESS(x)		(((x) & PAR_F) == 0)
/* When PAR_F == 0 (success) */
#define	PAR_SH_SHIFT		7
#define	PAR_SH_MASK		(0x3 << PAR_SH_SHIFT)
#define	PAR_NS_SHIFT		9
#define	PAR_NS_MASK		(0x3 << PAR_NS_SHIFT)
#define	PAR_PA_SHIFT		12
#define	PAR_PA_MASK		0x0000fffffffff000
#define	PAR_ATTR_SHIFT		56
#define	PAR_ATTR_MASK		(0xff << PAR_ATTR_SHIFT)
/* When PAR_F == 1 (aborted) */
#define	PAR_FST_SHIFT		1
#define	PAR_FST_MASK		(0x3f << PAR_FST_SHIFT)
#define	PAR_PTW_SHIFT		8
#define	PAR_PTW_MASK		(0x1 << PAR_PTW_SHIFT)
#define	PAR_S_SHIFT		9
#define	PAR_S_MASK		(0x1 << PAR_S_SHIFT)

/* SCTLR_EL1 - System Control Register */
#define	SCTLR_RES0	0xc8222440	/* Reserved ARMv8.0, write 0 */
#define	SCTLR_RES1	0x30d00800	/* Reserved ARMv8.0, write 1 */

#define	SCTLR_M		0x00000001
#define	SCTLR_A		0x00000002
#define	SCTLR_C		0x00000004
#define	SCTLR_SA	0x00000008
#define	SCTLR_SA0	0x00000010
#define	SCTLR_CP15BEN	0x00000020
/* Bit 6 is reserved */
#define	SCTLR_ITD	0x00000080
#define	SCTLR_SED	0x00000100
#define	SCTLR_UMA	0x00000200
/* Bit 10 is reserved */
/* Bit 11 is reserved */
#define	SCTLR_I		0x00001000
#define	SCTLR_EnDB	0x00002000 /* ARMv8.3 */
#define	SCTLR_DZE	0x00004000
#define	SCTLR_UCT	0x00008000
#define	SCTLR_nTWI	0x00010000
/* Bit 17 is reserved */
#define	SCTLR_nTWE	0x00040000
#define	SCTLR_WXN	0x00080000
/* Bit 20 is reserved */
#define	SCTLR_IESB	0x00200000 /* ARMv8.2 */
/* Bit 22 is reserved */
#define	SCTLR_SPAN	0x00800000 /* ARMv8.1 */
#define	SCTLR_EOE	0x01000000
#define	SCTLR_EE	0x02000000
#define	SCTLR_UCI	0x04000000
#define	SCTLR_EnDA	0x08000000 /* ARMv8.3 */
#define	SCTLR_nTLSMD	0x10000000 /* ARMv8.2 */
#define	SCTLR_LSMAOE	0x20000000 /* ARMv8.2 */
#define	SCTLR_EnIB	0x40000000 /* ARMv8.3 */
#define	SCTLR_EnIA	0x80000000 /* ARMv8.3 */

/* SPSR_EL1 */
/*
 * When the exception is taken in AArch64:
 * M[3:2] is the exception level
 * M[1]   is unused
 * M[0]   is the SP select:
 *         0: always SP0
 *         1: current ELs SP
 */
#define	PSR_M_EL0t	0x00000000
#define	PSR_M_EL1t	0x00000004
#define	PSR_M_EL1h	0x00000005
#define	PSR_M_EL2t	0x00000008
#define	PSR_M_EL2h	0x00000009
#define	PSR_M_64	0x00000000
#define	PSR_M_32	0x00000010
#define	PSR_M_MASK	0x0000000f

#define	PSR_T		0x00000020

#define	PSR_AARCH32	0x00000010
#define	PSR_F		0x00000040
#define	PSR_I		0x00000080
#define	PSR_A		0x00000100
#define	PSR_D		0x00000200
#define	PSR_DAIF	(PSR_D | PSR_A | PSR_I | PSR_F)
#define	PSR_IL		0x00100000
#define	PSR_SS		0x00200000
#define	PSR_V		0x10000000
#define	PSR_C		0x20000000
#define	PSR_Z		0x40000000
#define	PSR_N		0x80000000
#define	PSR_FLAGS	0xf0000000

/* TCR_EL1 - Translation Control Register */
#define	TCR_HD_SHIFT	40
#define	TCR_HD		(0x1UL << TCR_HD_SHIFT)
#define	TCR_HA_SHIFT	39
#define	TCR_HA		(0x1UL << TCR_HA_SHIFT)

#define	TCR_ASID_SHIFT	36
#define	TCR_ASID_WIDTH	1
#define	TCR_ASID_16	(0x1UL << TCR_ASID_SHIFT)

#define	TCR_IPS_SHIFT	32
#define	TCR_IPS_WIDTH	3
#define	TCR_IPS_32BIT	(0 << TCR_IPS_SHIFT)
#define	TCR_IPS_36BIT	(1 << TCR_IPS_SHIFT)
#define	TCR_IPS_40BIT	(2 << TCR_IPS_SHIFT)
#define	TCR_IPS_42BIT	(3 << TCR_IPS_SHIFT)
#define	TCR_IPS_44BIT	(4 << TCR_IPS_SHIFT)
#define	TCR_IPS_48BIT	(5 << TCR_IPS_SHIFT)

#define	TCR_TG1_SHIFT	30
#define	TCR_TG1_16K	(1 << TCR_TG1_SHIFT)
#define	TCR_TG1_4K	(2 << TCR_TG1_SHIFT)
#define	TCR_TG1_64K	(3 << TCR_TG1_SHIFT)

#define	TCR_SH1_SHIFT	28
#define	TCR_SH1_IS	(0x3UL << TCR_SH1_SHIFT)
#define	TCR_ORGN1_SHIFT	26
#define	TCR_ORGN1_WBWA	(0x1UL << TCR_ORGN1_SHIFT)
#define	TCR_IRGN1_SHIFT	24
#define	TCR_IRGN1_WBWA	(0x1UL << TCR_IRGN1_SHIFT)
#define	TCR_A1_SHIFT	22
#define	TCR_A1		(0x1UL << TCR_A1_SHIFT)
#define	TCR_SH0_SHIFT	12
#define	TCR_SH0_IS	(0x3UL << TCR_SH0_SHIFT)
#define	TCR_ORGN0_SHIFT	10
#define	TCR_ORGN0_WBWA	(0x1UL << TCR_ORGN0_SHIFT)
#define	TCR_IRGN0_SHIFT	8
#define	TCR_IRGN0_WBWA	(0x1UL << TCR_IRGN0_SHIFT)

#define	TCR_CACHE_ATTRS	((TCR_IRGN0_WBWA | TCR_IRGN1_WBWA) |\
				(TCR_ORGN0_WBWA | TCR_ORGN1_WBWA))

#ifdef SMP
#define	TCR_SMP_ATTRS	(TCR_SH0_IS | TCR_SH1_IS)
#else
#define	TCR_SMP_ATTRS	0
#endif

#define	TCR_T1SZ_SHIFT	16
#define	TCR_T0SZ_SHIFT	0
#define	TCR_T1SZ(x)	((x) << TCR_T1SZ_SHIFT)
#define	TCR_T0SZ(x)	((x) << TCR_T0SZ_SHIFT)
#define	TCR_TxSZ(x)	(TCR_T1SZ(x) | TCR_T0SZ(x))

/* Saved Program Status Register */
#define	DBG_SPSR_SS	(0x1 << 21)

/* Monitor Debug System Control Register */
#define	DBG_MDSCR_SS	(0x1 << 0)
#define	DBG_MDSCR_KDE	(0x1 << 13)
#define	DBG_MDSCR_MDE	(0x1 << 15)

/* Perfomance Monitoring Counters */
#define	PMCR_E		(1 << 0) /* Enable all counters */
#define	PMCR_P		(1 << 1) /* Reset all counters */
#define	PMCR_C		(1 << 2) /* Clock counter reset */
#define	PMCR_D		(1 << 3) /* CNTR counts every 64 clk cycles */
#define	PMCR_X		(1 << 4) /* Export to ext. monitoring (ETM) */
#define	PMCR_DP		(1 << 5) /* Disable CCNT if non-invasive debug*/
#define	PMCR_LC		(1 << 6) /* Long cycle count enable */
#define	PMCR_IMP_SHIFT	24 /* Implementer code */
#define	PMCR_IMP_MASK	(0xff << PMCR_IMP_SHIFT)
#define	PMCR_IDCODE_SHIFT	16 /* Identification code */
#define	PMCR_IDCODE_MASK	(0xff << PMCR_IDCODE_SHIFT)
#define	 PMCR_IDCODE_CORTEX_A57	0x01
#define	 PMCR_IDCODE_CORTEX_A72	0x02
#define	 PMCR_IDCODE_CORTEX_A53	0x03
#define	PMCR_N_SHIFT	11       /* Number of counters implemented */
#define	PMCR_N_MASK	(0x1f << PMCR_N_SHIFT)

#endif /* !_MACHINE_ARMREG_H_ */
