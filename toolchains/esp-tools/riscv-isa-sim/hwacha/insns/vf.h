if(!ENABLED) 
  h->take_exception(HWACHA_CAUSE_ILLEGAL_INSTRUCTION, VF_PC);

if (VL) {
  if (!h->vf_active()) {
    WRITE_VFRM(STATE.frm);
    WRITE_VF_PC(XS1 + insn.s_imm());
    for (uint32_t i=0; i<VL; i++)
      h->get_ut_state(i)->run = true;
  }

vf_loop:

  if (VF_PC & 7) {
#ifdef RISCV_ENABLE_HCOMMITLOG
    fprintf(stderr,"H: MISALIGNED VF INST\n");
#endif
    h->take_exception(HWACHA_CAUSE_VF_MISALIGNED_FETCH, VF_PC);
  }

  insn_t ut_insn = p->get_mmu()->load_insn(VF_PC).insn;

  bool matched = false;

  #define DECLARE_INSN(name, match, mask) \
    extern reg_t hwacha_##name(processor_t*, insn_t, reg_t); \
    if ((ut_insn.bits() & mask) == match) { \
      WRITE_VF_PC(hwacha_##name(p, ut_insn, VF_PC)); \
      matched = true; \
    }
  #include "opcodes_hwacha_ut.h"
  #undef DECLARE_INSN

  if (!matched) {
#ifdef RISCV_ENABLE_HCOMMITLOG
    fprintf(stderr,"H: ILL VF INST\n");
#endif
    h->take_exception(HWACHA_CAUSE_VF_ILLEGAL_INSTRUCTION, VF_PC);
  }

  if (!h->get_debug()) {
    if (h->vf_active())
      goto vf_loop;
  } else {
    fprintf(stderr, "vf block: 0x%016" PRIx64 " (0x%08" PRIx64 ") %s\n",
      VF_PC-8, ut_insn.bits(), h->get_ut_disassembler()->disassemble(ut_insn).c_str());
    if (h->vf_active())
      npc = pc;
  }
}
