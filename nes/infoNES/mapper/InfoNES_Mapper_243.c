/*===================================================================*/
/*                                                                   */
/*                      Mapper 243 (Pirates)                         */
/*                                                                   */
/*===================================================================*/

BYTE Map243_Regs[4];

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 243                                            */
/*-------------------------------------------------------------------*/
void Map243_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map243_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map243_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map0_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  pGData->SRAMBANK = pGData->SRAM;


  /* Set ROM Banks */
  pGData->ROMBANK0 = ROMPAGE( 0 );
  pGData->ROMBANK1 = ROMPAGE( 1 );
  pGData->ROMBANK2 = ROMPAGE( 2 );
  pGData->ROMBANK3 = ROMPAGE( 3 );

  /* Set PPU Banks */
  if ( NesHeader.byVRomSize > 0 )
  {
    int nPage; for( nPage = 0; nPage < 8; ++nPage )
    {
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    }
    InfoNES_SetupChr(pGData);
  }

  /* Initialize state registers */
  Map243_Regs[0] = Map243_Regs[1] = Map243_Regs[2] = Map243_Regs[3] = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 243 Write to Apu Function                                 */
/*-------------------------------------------------------------------*/
void Map243_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  if ( wAddr == 0x4100 )
  {
    Map243_Regs[0] = byData;
  }
  else if ( wAddr == 0x4101 )
  {
    switch ( Map243_Regs[0] & 0x07 )
    {
      case 0x02:
        Map243_Regs[1] = byData & 0x01;
        break;

      case 0x00:
      case 0x04:
      case 0x07:
        Map243_Regs[2] = ( byData & 0x01 ) << 1;
        break;

      /* Set ROM Banks */
      case 0x05:
        pGData->ROMBANK0 = ROMPAGE( ( byData * 4 + 0 ) % ( NesHeader.byRomSize << 1 ) );
        pGData->ROMBANK1 = ROMPAGE( ( byData * 4 + 1 ) % ( NesHeader.byRomSize << 1 ) );
        pGData->ROMBANK2 = ROMPAGE( ( byData * 4 + 2 ) % ( NesHeader.byRomSize << 1 ) );
        pGData->ROMBANK3 = ROMPAGE( ( byData * 4 + 3 ) % ( NesHeader.byRomSize << 1 ) );
        break;

      case 0x06:
        Map243_Regs[3] = ( byData & 0x03 ) << 2;
        break;
    }

    /* Set PPU Banks */
    if ( ( NesHeader.byVRomSize << 3 ) <= 64 )
    {
      BYTE chr_bank = ( Map243_Regs[2] + Map243_Regs[3] ) >> 1;

      pGData->PPUBANK[ 0 ] = VROMPAGE( ( chr_bank * 8 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( ( chr_bank * 8 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( ( chr_bank * 8 + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( ( chr_bank * 8 + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( ( chr_bank * 8 + 4 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( ( chr_bank * 8 + 5 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( ( chr_bank * 8 + 6 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( ( chr_bank * 8 + 7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
    else
    {
      BYTE chr_bank = Map243_Regs[1] + Map243_Regs[2] + Map243_Regs[3];

      pGData->PPUBANK[ 0 ] = VROMPAGE( ( chr_bank * 8 + 0 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 1 ] = VROMPAGE( ( chr_bank * 8 + 1 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 2 ] = VROMPAGE( ( chr_bank * 8 + 2 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 3 ] = VROMPAGE( ( chr_bank * 8 + 3 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 4 ] = VROMPAGE( ( chr_bank * 8 + 4 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 5 ] = VROMPAGE( ( chr_bank * 8 + 5 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 6 ] = VROMPAGE( ( chr_bank * 8 + 6 ) % ( NesHeader.byVRomSize << 3 ) );
      pGData->PPUBANK[ 7 ] = VROMPAGE( ( chr_bank * 8 + 7 ) % ( NesHeader.byVRomSize << 3 ) );
      InfoNES_SetupChr(pGData);
    }
  }
}
