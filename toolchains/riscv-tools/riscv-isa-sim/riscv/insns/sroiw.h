require_rv64;
require_extension(EXT_XBITMANIP);
WRITE_RD(sext32(~((~(uint32_t)RS1) >> SHAMT)));
