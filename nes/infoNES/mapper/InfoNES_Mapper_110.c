/*===================================================================*/
/*                                                                   */
/*                          Mapper 110                               */
/*                                                                   */
/*===================================================================*/

BYTE	Map110_Reg0, Map110_Reg1;

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 110                                            */
/*-------------------------------------------------------------------*/
void Map110_Init(Nes_Global_T *pGData)
{
  /* Initialize Mapper */
  MapperInit = Map110_Init;

  /* Write to Mapper */
  MapperWrite = Map0_Write;

  /* Write to SRAM */
  MapperSram = Map0_Sram;

  /* Write to APU */
  MapperApu = Map110_Apu;

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
  if ( NesHeader.byVRomSize > 0 ) {
    int nPage; for( nPage = 0; nPage < 8; ++nPage )
      pGData->PPUBANK[ nPage ] = VROMPAGE( nPage );
    InfoNES_SetupChr(pGData);
  }

  /* Initialize Registers */
  Map110_Reg0 = 0;
  Map110_Reg1 = 0;

  /* Set up wiring of the interrupt pin */
  K6502_Set_Int_Wiring( 1, 1 ); 
}

/*-------------------------------------------------------------------*/
/*  Mapper 110 Write to APU Function                                 */
/*-------------------------------------------------------------------*/
void Map110_Apu(Nes_Global_T *pGData, WORD wAddr, BYTE byData )
{
  switch( wAddr ) {
  case	0x4100:
    Map110_Reg1 = byData & 0x07;
    break;
  case	0x4101:
    switch( Map110_Reg1 ) {
    case	5:
      pGData->ROMBANK0 = ROMPAGE( ((byData << 2) + 0) % ( NesHeader.byRomSize << 1 ) );
      pGData->ROMBANK1 = ROMPAGE( ((byData << 2) + 1) % ( NesHeader.byRomSize << 1 ) );
      pGData->ROMBANK2 = ROMPAGE( ((byData << 2) + 2) % ( NesHeader.byRomSize << 1 ) );
      pGData->ROMBANK3 = ROMPAGE( ((byData << 2) + 3) % ( NesHeader.byRomSize << 1 ) );
      break;

    case	0:
      Map110_Reg0 = byData & 0x01;
      pGData->PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr(pGData);
      break;

    case	2:
      Map110_Reg0 = byData;
      pGData->PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr(pGData);
      break;

    case	4:
      Map110_Reg0 = Map110_Reg0 | (byData<<1);
      pGData->PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr(pGData);
      break;

    case	6:
      Map110_Reg0 = Map110_Reg0 | (byData<<2);
      pGData->PPUBANK[ 0 ] = VROMPAGE(((Map110_Reg0 << 3) + 0) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 1 ] = VROMPAGE(((Map110_Reg0 << 3) + 1) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 2 ] = VROMPAGE(((Map110_Reg0 << 3) + 2) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 3 ] = VROMPAGE(((Map110_Reg0 << 3) + 3) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 4 ] = VROMPAGE(((Map110_Reg0 << 3) + 4) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 5 ] = VROMPAGE(((Map110_Reg0 << 3) + 5) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 6 ] = VROMPAGE(((Map110_Reg0 << 3) + 6) % (NesHeader.byVRomSize<<3));
      pGData->PPUBANK[ 7 ] = VROMPAGE(((Map110_Reg0 << 3) + 7) % (NesHeader.byVRomSize<<3));
      InfoNES_SetupChr(pGData);
      break;

    default:
      break;
    }
    break;
  default:
    break;
  }
}
