if(xlen == 64)
  WRITE_SRD(SRS1 << VSHAMT);
else
{
  if(SHAMT & 0x20)
    throw trap_illegal_instruction(0);
  WRITE_SRD(sext32(SRS1 << VSHAMT));
}
