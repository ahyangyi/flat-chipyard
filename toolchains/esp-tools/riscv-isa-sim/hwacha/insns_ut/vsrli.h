if(xlen == 64)
  WRITE_SRD(SRS1 >> VSHAMT);
else
{
  if(VSHAMT & 0x20)
    throw trap_illegal_instruction(0);
  WRITE_SRD(sext32((uint32_t)SRS1 >> VSHAMT));
}
