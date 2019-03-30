/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2015 by Frédéric Coste

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*****************************************************************************
 *
 *   z80dasm.c
 *   Portable Z80 disassembler
 *
 *   Copyright Juergen Buchmueller, all rights reserved.
 *
 *   - This source code is released as freeware for non-commercial purposes.
 *   - You are free to use and redistribute this code in modified or
 *     unmodified form, provided you list me in the credits.
 *   - If you modify this source code, you must add a notice to each modified
 *     source file that it has been changed.  If you're a nice person, you
 *     will clearly mark each change too.  :)
 *   - If you wish to use this for commercial purposes, please contact me at
 *     pullmoll@t-online.de
 *   - The author of this copywritten work reserves the right to change the
 *     terms of its usage and license at any time, including retroactively
 *   - This entire notice must remain in the source code.
 *
 *****************************************************************************/


#include <vcl.h>
#pragma hdrstop
#include "TFormDebugZ80Editor.h"
#include "TFormDebugEditValue.h"
#include "TFormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


static const AnsiString cTitle = "Z80 editor";
static const AnsiString cTitlePaused  = " - [PAUSED]";


enum Z80Mnemonics
{
	zADC  , zADD  , zAND  , zBIT  , zCALL , zCCF  , zCP   , zCPD  ,
	zCPDR , zCPI  , zCPIR , zCPL  , zDAA  , zDB   , zDEC  , zDI   ,
	zDJNZ , zEI   , zEX   , zEXX  , zHLT  , zIM   , zIN   , zINC  ,
	zIND  , zINDR , zINI  , zINIR , zJP   , zJR   , zLD   , zLDD  ,
	zLDDR , zLDI  , zLDIR , zNEG  , zNOP  , zOR   , zOTDR , zOTIR ,
	zOUT  , zOUTD , zOUTI , zPOP  , zPUSH , zRES  , zRET  , zRETI ,
	zRETN , zRL   , zRLA  , zRLC  , zRLCA , zRLD  , zRR   , zRRA  ,
	zRRC  , zRRCA , zRRD  , zRST  , zSBC  , zSCF  , zSET  , zSLA  ,
	zSLL  , zSRA  , zSRL  , zSUB  , zXOR
};

static const char * Z80MnemonicStr[] =
{
	"adc" ,"add" ,"and" ,"bit" ,"call","ccf" ,"cp"  ,"cpd" ,
	"cpdr","cpi" ,"cpir","cpl" ,"daa" ,"db"  ,"dec" ,"di"  ,
	"djnz","ei"  ,"ex"  ,"exx" ,"halt","im"  ,"in"  ,"inc" ,
	"ind" ,"indr","ini" ,"inir","jp"  ,"jr"  ,"ld"  ,"ldd" ,
	"lddr","ldi" ,"ldir","neg" ,"nop" ,"or"  ,"otdr","otir",
	"out" ,"outd","outi","pop" ,"push","res" ,"ret" ,"reti",
	"retn","rl"  ,"rla" ,"rlc" ,"rlca","rld" ,"rr"  ,"rra" ,
	"rrc" ,"rrca","rrd" ,"rst" ,"sbc" ,"scf" ,"set" ,"sla" ,
	"sll" ,"sra" ,"srl" ,"sub" ,"xor "
};

struct Z80DASM
{
	unsigned char mnemonic;
	const char *arguments;
};


static const Z80DASM mnemonic_xx_cb[256] =
{
	{zRLC,"b=Y"},   {zRLC,"c=Y"},   {zRLC,"d=Y"},   {zRLC,"e=Y"},
	{zRLC,"H=Y"},   {zRLC,"l=Y"},   {zRLC,"Y"},     {zRLC,"a=Y"},
	{zRRC,"b=Y"},   {zRRC,"c=Y"},   {zRRC,"d=Y"},   {zRRC,"e=Y"},
	{zRRC,"h=Y"},   {zRRC,"l=Y"},   {zRRC,"Y"},     {zRRC,"a=Y"},
	{zRL,"b=Y"},    {zRL,"c=Y"},    {zRL,"d=Y"},    {zRL,"e=Y"},
	{zRL,"h=Y"},    {zRL,"l=Y"},    {zRL,"Y"},      {zRL,"a=Y"},
	{zRR,"b=Y"},    {zRR,"c=Y"},    {zRR,"d=Y"},    {zRR,"e=Y"},
	{zRR,"h=Y"},    {zRR,"l=Y"},    {zRR,"Y"},      {zRR,"a=Y"},
	{zSLA,"b=Y"},   {zSLA,"c=Y"},   {zSLA,"d=Y"},   {zSLA,"e=Y"},
	{zSLA,"h=Y"},   {zSLA,"l=Y"},   {zSLA,"Y"},     {zSLA,"a=Y"},
	{zSRA,"b=Y"},   {zSRA,"c=Y"},   {zSRA,"d=Y"},   {zSRA,"e=Y"},
	{zSRA,"h=Y"},   {zSRA,"l=Y"},   {zSRA,"Y"},     {zSRA,"a=Y"},
	{zSLL,"b=Y"},   {zSLL,"c=Y"},   {zSLL,"d=Y"},   {zSLL,"e=Y"},
	{zSLL,"h=Y"},   {zSLL,"l=Y"},   {zSLL,"Y"},     {zSLL,"a=Y"},
	{zSRL,"b=Y"},   {zSRL,"c=Y"},   {zSRL,"d=Y"},   {zSRL,"e=Y"},
	{zSRL,"h=Y"},   {zSRL,"l=Y"},   {zSRL,"Y"},     {zSRL,"a=Y"},
	{zBIT,"b=0,Y"}, {zBIT,"c=0,Y"}, {zBIT,"d=0,Y"}, {zBIT,"e=0,Y"},
	{zBIT,"h=0,Y"}, {zBIT,"l=0,Y"}, {zBIT,"0,Y"},   {zBIT,"a=0,Y"},
	{zBIT,"b=1,Y"}, {zBIT,"c=1,Y"}, {zBIT,"d=1,Y"}, {zBIT,"e=1,Y"},
	{zBIT,"h=1,Y"}, {zBIT,"l=1,Y"}, {zBIT,"1,Y"},   {zBIT,"a=1,Y"},
	{zBIT,"b=2,Y"}, {zBIT,"c=2,Y"}, {zBIT,"d=2,Y"}, {zBIT,"e=2,Y"},
	{zBIT,"h=2,Y"}, {zBIT,"l=2,Y"}, {zBIT,"2,Y"},   {zBIT,"a=2,Y"},
	{zBIT,"b=3,Y"}, {zBIT,"c=3,Y"}, {zBIT,"d=3,Y"}, {zBIT,"e=3,Y"},
	{zBIT,"h=3,Y"}, {zBIT,"l=3,Y"}, {zBIT,"3,Y"},   {zBIT,"a=3,Y"},
	{zBIT,"b=4,Y"}, {zBIT,"c=4,Y"}, {zBIT,"d=4,Y"}, {zBIT,"e=4,Y"},
	{zBIT,"h=4,Y"}, {zBIT,"l=4,Y"}, {zBIT,"4,Y"},   {zBIT,"a=4,Y"},
	{zBIT,"b=5,Y"}, {zBIT,"c=5,Y"}, {zBIT,"d=5,Y"}, {zBIT,"e=5,Y"},
	{zBIT,"h=5,Y"}, {zBIT,"l=5,Y"}, {zBIT,"5,Y"},   {zBIT,"a=5,Y"},
	{zBIT,"b=6,Y"}, {zBIT,"c=6,Y"}, {zBIT,"d=6,Y"}, {zBIT,"e=6,Y"},
	{zBIT,"h=6,Y"}, {zBIT,"l=6,Y"}, {zBIT,"6,Y"},   {zBIT,"a=6,Y"},
	{zBIT,"b=7,Y"}, {zBIT,"c=7,Y"}, {zBIT,"d=7,Y"}, {zBIT,"e=7,Y"},
	{zBIT,"h=7,Y"}, {zBIT,"l=7,Y"}, {zBIT,"7,Y"},   {zBIT,"a=7,Y"},
	{zRES,"b=0,Y"}, {zRES,"c=0,Y"}, {zRES,"d=0,Y"}, {zRES,"e=0,Y"},
	{zRES,"h=0,Y"}, {zRES,"l=0,Y"}, {zRES,"0,Y"},   {zRES,"a=0,Y"},
	{zRES,"b=1,Y"}, {zRES,"c=1,Y"}, {zRES,"d=1,Y"}, {zRES,"e=1,Y"},
	{zRES,"h=1,Y"}, {zRES,"l=1,Y"}, {zRES,"1,Y"},   {zRES,"a=1,Y"},
	{zRES,"b=2,Y"}, {zRES,"c=2,Y"}, {zRES,"d=2,Y"}, {zRES,"e=2,Y"},
	{zRES,"h=2,Y"}, {zRES,"l=2,Y"}, {zRES,"2,Y"},   {zRES,"a=2,Y"},
	{zRES,"b=3,Y"}, {zRES,"c=3,Y"}, {zRES,"d=3,Y"}, {zRES,"e=3,Y"},
	{zRES,"h=3,Y"}, {zRES,"l=3,Y"}, {zRES,"3,Y"},   {zRES,"a=3,Y"},
	{zRES,"b=4,Y"}, {zRES,"c=4,Y"}, {zRES,"d=4,Y"}, {zRES,"e=4,Y"},
	{zRES,"h=4,Y"}, {zRES,"l=4,Y"}, {zRES,"4,Y"},   {zRES,"a=4,Y"},
	{zRES,"b=5,Y"}, {zRES,"c=5,Y"}, {zRES,"d=5,Y"}, {zRES,"e=5,Y"},
	{zRES,"h=5,Y"}, {zRES,"l=5,Y"}, {zRES,"5,Y"},   {zRES,"a=5,Y"},
	{zRES,"b=6,Y"}, {zRES,"c=6,Y"}, {zRES,"d=6,Y"}, {zRES,"e=6,Y"},
	{zRES,"h=6,Y"}, {zRES,"l=6,Y"}, {zRES,"6,Y"},   {zRES,"a=6,Y"},
	{zRES,"b=7,Y"}, {zRES,"c=7,Y"}, {zRES,"d=7,Y"}, {zRES,"e=7,Y"},
	{zRES,"h=7,Y"}, {zRES,"l=7,Y"}, {zRES,"7,Y"},   {zRES,"a=7,Y"},
	{zSET,"b=0,Y"}, {zSET,"c=0,Y"}, {zSET,"d=0,Y"}, {zSET,"e=0,Y"},
	{zSET,"h=0,Y"}, {zSET,"l=0,Y"}, {zSET,"0,Y"},   {zSET,"a=0,Y"},
	{zSET,"b=1,Y"}, {zSET,"c=1,Y"}, {zSET,"d=1,Y"}, {zSET,"e=1,Y"},
	{zSET,"h=1,Y"}, {zSET,"l=1,Y"}, {zSET,"1,Y"},   {zSET,"a=1,Y"},
	{zSET,"b=2,Y"}, {zSET,"c=2,Y"}, {zSET,"d=2,Y"}, {zSET,"e=2,Y"},
	{zSET,"h=2,Y"}, {zSET,"l=2,Y"}, {zSET,"2,Y"},   {zSET,"a=2,Y"},
	{zSET,"b=3,Y"}, {zSET,"c=3,Y"}, {zSET,"d=3,Y"}, {zSET,"e=3,Y"},
	{zSET,"h=3,Y"}, {zSET,"l=3,Y"}, {zSET,"3,Y"},   {zSET,"a=3,Y"},
	{zSET,"b=4,Y"}, {zSET,"c=4,Y"}, {zSET,"d=4,Y"}, {zSET,"e=4,Y"},
	{zSET,"h=4,Y"}, {zSET,"l=4,Y"}, {zSET,"4,Y"},   {zSET,"a=4,Y"},
	{zSET,"b=5,Y"}, {zSET,"c=5,Y"}, {zSET,"d=5,Y"}, {zSET,"e=5,Y"},
	{zSET,"h=5,Y"}, {zSET,"l=5,Y"}, {zSET,"5,Y"},   {zSET,"a=5,Y"},
	{zSET,"b=6,Y"}, {zSET,"c=6,Y"}, {zSET,"d=6,Y"}, {zSET,"e=6,Y"},
	{zSET,"h=6,Y"}, {zSET,"l=6,Y"}, {zSET,"6,Y"},   {zSET,"a=6,Y"},
	{zSET,"b=7,Y"}, {zSET,"c=7,Y"}, {zSET,"d=7,Y"}, {zSET,"e=7,Y"},
	{zSET,"h=7,Y"}, {zSET,"l=7,Y"}, {zSET,"7,Y"},   {zSET,"a=7,Y"}
};

static const Z80DASM mnemonic_cb[256] =
{
	{zRLC,"b"},     {zRLC,"c"},     {zRLC,"d"},      {zRLC,"e"},
	{zRLC,"h"},     {zRLC,"l"},     {zRLC,"(hl)H"},  {zRLC,"a"},
	{zRRC,"b"},     {zRRC,"c"},     {zRRC,"d"},      {zRRC,"e"},
	{zRRC,"h"},     {zRRC,"l"},     {zRRC,"(hl)H"},  {zRRC,"a"},
	{zRL,"b"},      {zRL,"c"},      {zRL,"d"},       {zRL,"e"},
	{zRL,"h"},      {zRL,"l"},      {zRL,"(hl)H"},   {zRL,"a"},
	{zRR,"b"},      {zRR,"c"},      {zRR,"d"},       {zRR,"e"},
	{zRR,"h"},      {zRR,"l"},      {zRR,"(hl)H"},   {zRR,"a"},
	{zSLA,"b"},     {zSLA,"c"},     {zSLA,"d"},      {zSLA,"e"},
	{zSLA,"h"},     {zSLA,"l"},     {zSLA,"(hl)H"},  {zSLA,"a"},
	{zSRA,"b"},     {zSRA,"c"},     {zSRA,"d"},      {zSRA,"e"},
	{zSRA,"h"},     {zSRA,"l"},     {zSRA,"(hl)H"},  {zSRA,"a"},
	{zSLL,"b"},     {zSLL,"c"},     {zSLL,"d"},      {zSLL,"e"},
	{zSLL,"h"},     {zSLL,"l"},     {zSLL,"(hl)H"},  {zSLL,"a"},
	{zSRL,"b"},     {zSRL,"c"},     {zSRL,"d"},      {zSRL,"e"},
	{zSRL,"h"},     {zSRL,"l"},     {zSRL,"(hl)H"},  {zSRL,"a"},
	{zBIT,"0,b"},   {zBIT,"0,c"},   {zBIT,"0,d"},    {zBIT,"0,e"},
	{zBIT,"0,h"},   {zBIT,"0,l"},   {zBIT,"0,(hl)H"},{zBIT,"0,a"},
	{zBIT,"1,b"},   {zBIT,"1,c"},   {zBIT,"1,d"},    {zBIT,"1,e"},
	{zBIT,"1,h"},   {zBIT,"1,l"},   {zBIT,"1,(hl)H"},{zBIT,"1,a"},
	{zBIT,"2,b"},   {zBIT,"2,c"},   {zBIT,"2,d"},    {zBIT,"2,e"},
	{zBIT,"2,h"},   {zBIT,"2,l"},   {zBIT,"2,(hl)H"},{zBIT,"2,a"},
	{zBIT,"3,b"},   {zBIT,"3,c"},   {zBIT,"3,d"},    {zBIT,"3,e"},
	{zBIT,"3,h"},   {zBIT,"3,l"},   {zBIT,"3,(hl)H"},{zBIT,"3,a"},
	{zBIT,"4,b"},   {zBIT,"4,c"},   {zBIT,"4,d"},    {zBIT,"4,e"},
	{zBIT,"4,h"},   {zBIT,"4,l"},   {zBIT,"4,(hl)H"},{zBIT,"4,a"},
	{zBIT,"5,b"},   {zBIT,"5,c"},   {zBIT,"5,d"},    {zBIT,"5,e"},
	{zBIT,"5,h"},   {zBIT,"5,l"},   {zBIT,"5,(hl)H"},{zBIT,"5,a"},
	{zBIT,"6,b"},   {zBIT,"6,c"},   {zBIT,"6,d"},    {zBIT,"6,e"},
	{zBIT,"6,h"},   {zBIT,"6,l"},   {zBIT,"6,(hl)H"},{zBIT,"6,a"},
	{zBIT,"7,b"},   {zBIT,"7,c"},   {zBIT,"7,d"},    {zBIT,"7,e"},
	{zBIT,"7,h"},   {zBIT,"7,l"},   {zBIT,"7,(hl)H"},{zBIT,"7,a"},
	{zRES,"0,b"},   {zRES,"0,c"},   {zRES,"0,d"},    {zRES,"0,e"},
	{zRES,"0,h"},   {zRES,"0,l"},   {zRES,"0,(hl)H"},{zRES,"0,a"},
	{zRES,"1,b"},   {zRES,"1,c"},   {zRES,"1,d"},    {zRES,"1,e"},
	{zRES,"1,h"},   {zRES,"1,l"},   {zRES,"1,(hl)H"},{zRES,"1,a"},
	{zRES,"2,b"},   {zRES,"2,c"},   {zRES,"2,d"},    {zRES,"2,e"},
	{zRES,"2,h"},   {zRES,"2,l"},   {zRES,"2,(hl)H"},{zRES,"2,a"},
	{zRES,"3,b"},   {zRES,"3,c"},   {zRES,"3,d"},    {zRES,"3,e"},
	{zRES,"3,h"},   {zRES,"3,l"},   {zRES,"3,(hl)H"},{zRES,"3,a"},
	{zRES,"4,b"},   {zRES,"4,c"},   {zRES,"4,d"},    {zRES,"4,e"},
	{zRES,"4,h"},   {zRES,"4,l"},   {zRES,"4,(hl)H"},{zRES,"4,a"},
	{zRES,"5,b"},   {zRES,"5,c"},   {zRES,"5,d"},    {zRES,"5,e"},
	{zRES,"5,h"},   {zRES,"5,l"},   {zRES,"5,(hl)H"},{zRES,"5,a"},
	{zRES,"6,b"},   {zRES,"6,c"},   {zRES,"6,d"},    {zRES,"6,e"},
	{zRES,"6,h"},   {zRES,"6,l"},   {zRES,"6,(hl)H"},{zRES,"6,a"},
	{zRES,"7,b"},   {zRES,"7,c"},   {zRES,"7,d"},    {zRES,"7,e"},
	{zRES,"7,h"},   {zRES,"7,l"},   {zRES,"7,(hl)H"},{zRES,"7,a"},
	{zSET,"0,b"},   {zSET,"0,c"},   {zSET,"0,d"},    {zSET,"0,e"},
	{zSET,"0,h"},   {zSET,"0,l"},   {zSET,"0,(hl)H"},{zSET,"0,a"},
	{zSET,"1,b"},   {zSET,"1,c"},   {zSET,"1,d"},    {zSET,"1,e"},
	{zSET,"1,h"},   {zSET,"1,l"},   {zSET,"1,(hl)H"},{zSET,"1,a"},
	{zSET,"2,b"},   {zSET,"2,c"},   {zSET,"2,d"},    {zSET,"2,e"},
	{zSET,"2,h"},   {zSET,"2,l"},   {zSET,"2,(hl)H"},{zSET,"2,a"},
	{zSET,"3,b"},   {zSET,"3,c"},   {zSET,"3,d"},    {zSET,"3,e"},
	{zSET,"3,h"},   {zSET,"3,l"},   {zSET,"3,(hl)H"},{zSET,"3,a"},
	{zSET,"4,b"},   {zSET,"4,c"},   {zSET,"4,d"},    {zSET,"4,e"},
	{zSET,"4,h"},   {zSET,"4,l"},   {zSET,"4,(hl)H"},{zSET,"4,a"},
	{zSET,"5,b"},   {zSET,"5,c"},   {zSET,"5,d"},    {zSET,"5,e"},
	{zSET,"5,h"},   {zSET,"5,l"},   {zSET,"5,(hl)H"},{zSET,"5,a"},
	{zSET,"6,b"},   {zSET,"6,c"},   {zSET,"6,d"},    {zSET,"6,e"},
	{zSET,"6,h"},   {zSET,"6,l"},   {zSET,"6,(hl)H"},{zSET,"6,a"},
	{zSET,"7,b"},   {zSET,"7,c"},   {zSET,"7,d"},    {zSET,"7,e"},
	{zSET,"7,h"},   {zSET,"7,l"},   {zSET,"7,(hl)H"},{zSET,"7,a"}
};

static const Z80DASM mnemonic_ed[256] =
{
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zIN,"b,(c)"},  {zOUT,"(c),b"}, {zSBC,"hl,bc"}, {zLD,"(W),bc"},
	{zNEG,0},       {zRETN,0},      {zIM,"0"},      {zLD,"i,a"},
	{zIN,"c,(c)"},  {zOUT,"(c),c"}, {zADC,"hl,bc"}, {zLD,"bc,(W)"},
	{zNEG,"*"},     {zRETI,0},      {zIM,"0"},      {zLD,"r,a"},
	{zIN,"d,(c)"},  {zOUT,"(c),d"}, {zSBC,"hl,de"}, {zLD,"(W),de"},
	{zNEG,"*"},     {zRETN,0},      {zIM,"1"},      {zLD,"a,i"},
	{zIN,"e,(c)"},  {zOUT,"(c),e"}, {zADC,"hl,de"}, {zLD,"de,(W)"},
	{zNEG,"*"},     {zRETI,0},      {zIM,"2"},      {zLD,"a,r"},
	{zIN,"h,(c)"},  {zOUT,"(c),h"}, {zSBC,"hl,hl"}, {zLD,"(W),hl"},
	{zNEG,"*"},     {zRETN,0},      {zIM,"0"},      {zRRD,"(hl)"},
	{zIN,"l,(c)"},  {zOUT,"(c),l"}, {zADC,"hl,hl"}, {zLD,"hl,(W)"},
	{zNEG,"*"},     {zRETI,0},      {zIM,"0"},      {zRLD,"(hl)"},
	{zIN,"0,(c)"},  {zOUT,"(c),0"}, {zSBC,"hl,sp"}, {zLD,"(W),sp"},
	{zNEG,"*"},     {zRETN,0},      {zIM,"1"},      {zDB,"?"},
	{zIN,"a,(c)"},  {zOUT,"(c),a"}, {zADC,"hl,sp"}, {zLD,"sp,(W)"},
	{zNEG,"*"},     {zRETI,0},      {zIM,"2"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zLDI,0},       {zCPI,0},       {zINI,0},       {zOUTI,0},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zLDD,0},       {zCPD,0},       {zIND,0},       {zOUTD,0},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zLDIR,0},      {zCPIR,0},      {zINIR,0},      {zOTIR,0},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zLDDR,0},      {zCPDR,0},      {zINDR,0},      {zOTDR,0},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"}
};

static const Z80DASM mnemonic_xx[256] =
{
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zADD,"I,bc"},  {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zADD,"I,de"},  {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zLD,"I,N"},    {zLD,"(W),I"},  {zINC,"I"},
	{zINC,"Ih"},    {zDEC,"Ih"},    {zLD,"Ih,B"},   {zDB,"?"},
	{zDB,"?"},      {zADD,"I,I"},   {zLD,"I,(W)"},  {zDEC,"I"},
	{zINC,"Il"},    {zDEC,"Il"},    {zLD,"Il,B"},   {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zINC,"X"},     {zDEC,"X"},     {zLD,"X,B"},    {zDB,"?"},
	{zDB,"?"},      {zADD,"I,sp"},  {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zLD,"b,Ih"},   {zLD,"b,Il"},   {zLD,"b,X"},    {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zLD,"c,Ih"},   {zLD,"c,Il"},   {zLD,"c,X"},    {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zLD,"d,Ih"},   {zLD,"d,Il"},   {zLD,"d,X"},    {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zLD,"e,Ih"},   {zLD,"e,Il"},   {zLD,"e,X"},    {zDB,"?"},
	{zLD,"Ih,b"},   {zLD,"Ih,c"},   {zLD,"Ih,d"},   {zLD,"Ih,e"},
	{zLD,"Ih,Ih"},  {zLD,"Ih,Il"},  {zLD,"h,X"},    {zLD,"Ih,a"},
	{zLD,"Il,b"},   {zLD,"Il,c"},   {zLD,"Il,d"},   {zLD,"Il,e"},
	{zLD,"Il,Ih"},  {zLD,"Il,Il"},  {zLD,"l,X"},    {zLD,"Il,a"},
	{zLD,"X,b"},    {zLD,"X,c"},    {zLD,"X,d"},    {zLD,"X,e"},
	{zLD,"X,h"},    {zLD,"X,l"},    {zDB,"?"},      {zLD,"X,a"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zLD,"a,Ih"},   {zLD,"a,Il"},   {zLD,"a,X"},    {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zADD,"a,Ih"},  {zADD,"a,Il"},  {zADD,"a,X"},   {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zADC,"a,Ih"},  {zADC,"a,Il"},  {zADC,"a,X"},   {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zSUB,"Ih"},    {zSUB,"Il"},    {zSUB,"X"},     {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zSBC,"a,Ih"},  {zSBC,"a,Il"},  {zSBC,"a,X"},   {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zAND,"Ih"},    {zAND,"Il"},    {zAND,"X"},     {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zXOR,"Ih"},    {zXOR,"Il"},    {zXOR,"X"},     {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zOR,"Ih"},     {zOR,"Il"},     {zOR,"X"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zCP,"Ih"},     {zCP,"Il"},     {zCP,"X"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"cb"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zPOP,"I"},     {zDB,"?"},      {zEX,"(sp),I"},
	{zDB,"?"},      {zPUSH,"I"},    {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zJP,"(I)"},    {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zLD,"sp,I"},   {zDB,"?"},      {zDB,"?"},
	{zDB,"?"},      {zDB,"?"},      {zDB,"?"},      {zDB,"?"}
};

static const Z80DASM mnemonic_main[256] =
{
	{zNOP,0},       {zLD,"bc,N"},   {zLD,"(bc),a"}, {zINC,"bc"},
	{zINC,"b"},     {zDEC,"b"},     {zLD,"b,B"},    {zRLCA,0},
	{zEX,"af,af'"}, {zADD,"hl,bc"}, {zLD,"a,(bc)"}, {zDEC,"bc"},
	{zINC,"c"},     {zDEC,"c"},     {zLD,"c,B"},    {zRRCA,0},
	{zDJNZ,"O"},    {zLD,"de,N"},   {zLD,"(de),a"}, {zINC,"de"},
	{zINC,"d"},     {zDEC,"d"},     {zLD,"d,B"},    {zRLA,0},
	{zJR,"O"},      {zADD,"hl,de"}, {zLD,"a,(de)"}, {zDEC,"de"},
	{zINC,"e"},     {zDEC,"e"},     {zLD,"e,B"},    {zRRA,0},
	{zJR,"nz,O"},   {zLD,"hl,N"},   {zLD,"(W),hl"}, {zINC,"hl"},
	{zINC,"h"},     {zDEC,"h"},     {zLD,"h,B"},    {zDAA,0},
	{zJR,"z,O"},    {zADD,"hl,hl"}, {zLD,"hl,(W)"}, {zDEC,"hl"},
	{zINC,"l"},     {zDEC,"l"},     {zLD,"l,B"},    {zCPL,0},
	{zJR,"nc,O"},   {zLD,"sp,N"},   {zLD,"(W),a"},  {zINC,"sp"},
	{zINC,"(hl)H"}, {zDEC,"(hl)H"}, {zLD,"(hl),B"}, {zSCF,0},
	{zJR,"c,O"},    {zADD,"hl,sp"}, {zLD,"a,(W)"},  {zDEC,"sp"},
	{zINC,"a"},     {zDEC,"a"},     {zLD,"a,B"},    {zCCF,0},
	{zLD,"b,b"},    {zLD,"b,c"},    {zLD,"b,d"},    {zLD,"b,e"},
	{zLD,"b,h"},    {zLD,"b,l"},    {zLD,"b,(hl)"}, {zLD,"b,a"},
	{zLD,"c,b"},    {zLD,"c,c"},    {zLD,"c,d"},    {zLD,"c,e"},
	{zLD,"c,h"},    {zLD,"c,l"},    {zLD,"c,(hl)"}, {zLD,"c,a"},
	{zLD,"d,b"},    {zLD,"d,c"},    {zLD,"d,d"},    {zLD,"d,e"},
	{zLD,"d,h"},    {zLD,"d,l"},    {zLD,"d,(hl)"}, {zLD,"d,a"},
	{zLD,"e,b"},    {zLD,"e,c"},    {zLD,"e,d"},    {zLD,"e,e"},
	{zLD,"e,h"},    {zLD,"e,l"},    {zLD,"e,(hl)"}, {zLD,"e,a"},
	{zLD,"h,b"},    {zLD,"h,c"},    {zLD,"h,d"},    {zLD,"h,e"},
	{zLD,"h,h"},    {zLD,"h,l"},    {zLD,"h,(hl)"}, {zLD,"h,a"},
	{zLD,"l,b"},    {zLD,"l,c"},    {zLD,"l,d"},    {zLD,"l,e"},
	{zLD,"l,h"},    {zLD,"l,l"},    {zLD,"l,(hl)"}, {zLD,"l,a"},
	{zLD,"(hl),b"}, {zLD,"(hl),c"}, {zLD,"(hl),d"}, {zLD,"(hl),e"},
	{zLD,"(hl),h"}, {zLD,"(hl),l"}, {zHLT,0},       {zLD,"(hl),a"},
	{zLD,"a,b"},    {zLD,"a,c"},    {zLD,"a,d"},    {zLD,"a,e"},
	{zLD,"a,h"},    {zLD,"a,l"},    {zLD,"a,(hl)"}, {zLD,"a,a"},
	{zADD,"a,b"},   {zADD,"a,c"},   {zADD,"a,d"},   {zADD,"a,e"},
	{zADD,"a,h"},   {zADD,"a,l"},   {zADD,"a,(hl)"},{zADD,"a,a"},
	{zADC,"a,b"},   {zADC,"a,c"},   {zADC,"a,d"},   {zADC,"a,e"},
	{zADC,"a,h"},   {zADC,"a,l"},   {zADC,"a,(hl)"},{zADC,"a,a"},
	{zSUB,"b"},     {zSUB,"c"},     {zSUB,"d"},     {zSUB,"e"},
	{zSUB,"h"},     {zSUB,"l"},     {zSUB,"(hl)"},  {zSUB,"a"},
	{zSBC,"a,b"},   {zSBC,"a,c"},   {zSBC,"a,d"},   {zSBC,"a,e"},
	{zSBC,"a,h"},   {zSBC,"a,l"},   {zSBC,"a,(hl)"},{zSBC,"a,a"},
	{zAND,"b"},     {zAND,"c"},     {zAND,"d"},     {zAND,"e"},
	{zAND,"h"},     {zAND,"l"},     {zAND,"(hl)H"}, {zAND,"a"},
	{zXOR,"b"},     {zXOR,"c"},     {zXOR,"d"},     {zXOR,"e"},
	{zXOR,"h"},     {zXOR,"l"},     {zXOR,"(hl)H"}, {zXOR,"a"},
	{zOR,"b"},      {zOR,"c"},      {zOR,"d"},      {zOR,"e"},
	{zOR,"h"},      {zOR,"l"},      {zOR,"(hl)H"},  {zOR,"a"},
	{zCP,"b"},      {zCP,"c"},      {zCP,"d"},      {zCP,"e"},
	{zCP,"h"},      {zCP,"l"},      {zCP,"(hl)H"},  {zCP,"a"},
	{zRET,"nz"},    {zPOP,"bc"},    {zJP,"nz,A"},   {zJP,"A"},
	{zCALL,"nz,A"}, {zPUSH,"bc"},   {zADD,"a,B"},   {zRST,"V"},
	{zRET,"z"},     {zRET,0},       {zJP,"z,A"},    {zDB,"cb"},
	{zCALL,"z,A"},  {zCALL,"A"},    {zADC,"a,B"},   {zRST,"V"},
	{zRET,"nc"},    {zPOP,"de"},    {zJP,"nc,A"},   {zOUT,"(P),a"},
	{zCALL,"nc,A"}, {zPUSH,"de"},   {zSUB,"B"},     {zRST,"V"},
	{zRET,"c"},     {zEXX,0},       {zJP,"c,A"},    {zIN,"a,(P)"},
	{zCALL,"c,A"},  {zDB,"dd"},     {zSBC,"a,B"},   {zRST,"V"},
	{zRET,"po"},    {zPOP,"hl"},    {zJP,"po,A"},   {zEX,"(sp),hl"},
	{zCALL,"po,A"}, {zPUSH,"hl"},   {zAND,"B"},     {zRST,"V"},
	{zRET,"pe"},    {zJP,"(hl)"},   {zJP,"pe,A"},   {zEX,"de,hl"},
	{zCALL,"pe,A"}, {zDB,"ed"},     {zXOR,"B"},     {zRST,"V"},
	{zRET,"p"},     {zPOP,"af"},    {zJP,"p,A"},    {zDI,0},
	{zCALL,"p,A"},  {zPUSH,"af"},   {zOR,"B"},      {zRST,"V"},
	{zRET,"m"},     {zLD,"sp,hl"},  {zJP,"m,A"},    {zEI,0},
	{zCALL,"m,A"},  {zDB,"fd"},     {zCP,"B"},      {zRST,"V"}
};



TFormDebugZ80Editor *FormDebugZ80Editor;

//---------------------------------------------------------------------------
__fastcall TFormDebugZ80Editor::TFormDebugZ80Editor(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mNextInstructions = NULL;
  mPaused = false;

  // Apply Theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  PanelFormatBreakpoint1->Font->Color = clBlack;
  PanelBreakpoint1->Font->Color = clBlack;
  PanelFormatRegAF->Font->Color = clBlack;
  PanelRegA->Font->Color = clBlack;
  PanelRegF->Font->Color = clBlack;
  PanelFormatRegBC->Font->Color = clBlack;
  PanelRegB->Font->Color = clBlack;
  PanelRegC->Font->Color = clBlack;
  PanelFormatRegDE->Font->Color = clBlack;
  PanelRegD->Font->Color = clBlack;
  PanelRegE->Font->Color = clBlack;
  PanelFormatRegHL->Font->Color = clBlack;
  PanelRegH->Font->Color = clBlack;
  PanelRegL->Font->Color = clBlack;
  PanelFormatRegAFAlt->Font->Color = clBlack;
  PanelRegAAlt->Font->Color = clBlack;
  PanelRegFAlt->Font->Color = clBlack;
  PanelFormatRegBCAlt->Font->Color = clBlack;
  PanelRegBAlt->Font->Color = clBlack;
  PanelRegCAlt->Font->Color = clBlack;
  PanelFormatRegDEAlt->Font->Color = clBlack;
  PanelRegDAlt->Font->Color = clBlack;
  PanelRegEAlt->Font->Color = clBlack;
  PanelFormatRegHLAlt->Font->Color = clBlack;
  PanelRegHAlt->Font->Color = clBlack;
  PanelRegLAlt->Font->Color = clBlack;
  PanelFormatRegIX->Font->Color = clBlack;
  PanelRegIX->Font->Color = clBlack;
  PanelFormatRegIY->Font->Color = clBlack;
  PanelRegIY->Font->Color = clBlack;
  PanelFormatRegSP->Font->Color = clBlack;
  PanelRegSP->Font->Color = clBlack;
  PanelFormatRegPC->Font->Color = clBlack;
  PanelRegPC->Font->Color = clBlack;
  PanelFormatRegI->Font->Color = clBlack;
  PanelRegI->Font->Color = clBlack;
  PanelFormatRegR->Font->Color = clBlack;
  PanelRegR->Font->Color = clBlack;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugZ80Editor::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  StringGridInstructions->RowCount = 14;
  StringGridInstructions->ColWidths[0] = 50;
  StringGridInstructions->ColWidths[1] = 100;
  StringGridInstructions->ColWidths[3] = 100;
  StringGridInstructions->ColWidths[2] = StringGridInstructions->Width -
        (StringGridInstructions->ColWidths[0]+StringGridInstructions->ColWidths[1]+StringGridInstructions->ColWidths[3]);

  StringGridStack->RowCount = 14;
  StringGridStack->ColWidths[0] = 50;
  StringGridStack->ColWidths[1] = StringGridStack->Width - StringGridStack->ColWidths[0];

  // Subscribe to step by step event
  FormMain->SubscribeToCPCEvents(OnStepByStepEvent);

  mNextInstructions = new tULong[StringGridInstructions->RowCount];

  UpdateRegisters(FormMain->NativeCPC);
  UpdateFlags(FormMain->NativeCPC);
  UpdateInstructions(FormMain->NativeCPC);
  UpdateStack(FormMain->NativeCPC);
  UpdateInterrupt(FormMain->NativeCPC);
  InitBreakpoint(FormMain->NativeCPC);

  mPaused = FormMain->NativeCPC->paused;
  Caption = Title;

	UpdateRegistersEdition(mPaused);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugZ80Editor::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnStepByStepEvent);
  }

  delete [] mNextInstructions;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormDebugZ80Editor::GetTitle(void)
{
  AnsiString Title = cTitle;
  if (mPaused == true) Title += cTitlePaused;
 return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugZ80Editor::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->Z80FormLeft != 0)
       || (mSettingsP->Z80FormTop != 0)
       /*|| (mSettings->Z80FormWidth != 0)
       || (mSettings->Z80FormHeight != 0)*/ )
  {
    Position = poDesigned;
    Left = mSettingsP->Z80FormLeft;
    Top = mSettingsP->Z80FormTop;
    //Width = mSettings->Z80FormWidth;
    //Height = mSettings->Z80FormHeight;
  }

  ShowHint = mSettingsP->ShowHints;

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Color = mSettingsP->ThemeText1Color;
  PanelIFF1->Font->Color = mSettingsP->ThemeText2Color;
  PanelIFF2->Font->Color = mSettingsP->ThemeText2Color;
  StringGridInstructions->Font->Color = mSettingsP->ThemeText2Color;
  StringGridStack->Font->Color = mSettingsP->ThemeText2Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugZ80Editor::FormResize(TObject *Sender)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->Z80FormWidth = Width;
    mSettingsP->Z80FormHeight = Height;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->Z80FormLeft = Left;
    mSettingsP->Z80FormTop = Top;
  }

	NOT_USED(Message);
}
//---------------------------------------------------------------------------


tULong __fastcall TFormDebugZ80Editor::GetInstruction(tNativeCPC* NativeCPC,
                                                      tULong addr,
                                                      AnsiString& MachineCode,
                                                      AnsiString& Instruction,
                                                      AnsiString& MemoryContent)
{
const Z80DASM *d;
const char *ixy;
tUShort ea;
tUShort pc;
tUChar OpCode;
tChar offset;

  pc = (tUShort)addr;
	OpCode = Engine_ReadMem(NativeCPC, addr++);
  MachineCode = AnsiString::IntToHex(OpCode, 2) + " ";

  d = NULL;
  while (d == NULL)
  {
	  switch (OpCode)
	  {
	    case 0xcb:
      {
        OpCode = Engine_ReadMem(NativeCPC, addr++);
        MachineCode = AnsiString::IntToHex(OpCode, 2) + " ";
		    d = &mnemonic_cb[OpCode];
      }
		  break;

	    case 0xed:
      {
        OpCode = Engine_ReadMem(NativeCPC, addr++);
        MachineCode = AnsiString::IntToHex(OpCode, 2) + " ";
		    d = &mnemonic_ed[OpCode];
      }
		  break;

	    case 0xdd:
      {
		    ixy = "ix";
        OpCode = Engine_ReadMem(NativeCPC, addr++);
        MachineCode = AnsiString::IntToHex(OpCode, 2) + " ";
		    if (OpCode == 0xcb)
		    {
			    //offset = (tChar)Engine_ReadMem(NativeCPC, addr++);
          OpCode = Engine_ReadMem(NativeCPC, addr++);
          MachineCode = AnsiString::IntToHex(OpCode, 2) + " ";
			    d = &mnemonic_xx_cb[OpCode];
		    }
		    else
        {
          d = &mnemonic_xx[OpCode];
        }
      }
      break;

	    case 0xfd:
      {
		    ixy = "iy";
        OpCode = Engine_ReadMem(NativeCPC, addr++);
        MachineCode = AnsiString::IntToHex(OpCode, 2) + " ";
		    if (OpCode == 0xcb)
		    {
			    //offset = (tChar)Engine_ReadMem(NativeCPC, addr++);
          OpCode = Engine_ReadMem(NativeCPC, addr++);
          MachineCode = AnsiString::IntToHex(OpCode, 2) + " ";
			    d = &mnemonic_xx_cb[OpCode];
		    }
		    else
        {
          d = &mnemonic_xx[OpCode];
        }
      }
		  break;

	    default:
      {
		    d = &mnemonic_main[OpCode];
      }
		  break;
    }
	}

  // Add mnemonic
  Instruction = AnsiString(Z80MnemonicStr[d->mnemonic]);
  Instruction += " ";
  MemoryContent = "";

  // Add arguments
	if (d->arguments)
	{
    const char *src = d->arguments;
		while (*src)
		{
			switch (*src)
			{
			  case '?':   /* illegal opcode */
        {
          Instruction += "ILLEGAL " + MachineCode;
        }
        break;

			  case 'A':
        {
          ea = Engine_ReadMem(NativeCPC, addr++);
          ea += Engine_ReadMem(NativeCPC, addr++) << 8;
          Instruction += "&" + AnsiString::IntToHex(ea, 4);
          MachineCode += AnsiString::IntToHex(ea & 0xff, 2) + " " + AnsiString::IntToHex((ea >> 8) & 0xff, 2) + " ";
        }
        break;

			  case 'B':   /* Byte op arg */
        {
          ea = Engine_ReadMem(NativeCPC, addr++);
          Instruction += "&" + AnsiString::IntToHex(ea, 2);
          MachineCode += AnsiString::IntToHex(ea, 2) + " ";
        }
				break;

			  case 'N':   /* Immediate 16 bit */
        {
          ea = Engine_ReadMem(NativeCPC, addr++);
          ea += Engine_ReadMem(NativeCPC, addr++) << 8;
          Instruction += "&" + AnsiString::IntToHex(ea, 4);
          MachineCode += AnsiString::IntToHex(ea & 0xff, 2) + " " + AnsiString::IntToHex((ea >> 8) & 0xff, 2) + " ";
        }
				break;

  			case 'O':   /* Offset relative to PC */
        {
				  offset = (tChar)Engine_ReadMem(NativeCPC, addr++);;
          Instruction += "&" + AnsiString::IntToHex((pc + offset + 2) & 0xffff, 4);
          MachineCode += AnsiString::IntToHex(offset & 0xff, 2) + " ";
        }
				break;

			  case 'P':   /* Port number */
        {
          ea = Engine_ReadMem(NativeCPC, addr++);
          Instruction += "&" + AnsiString::IntToHex(ea, 2);
          MachineCode += AnsiString::IntToHex(ea, 2) + " ";
        }
				break;

			  case 'V':   /* Restart vector */
        {
				  ea = OpCode & 0x38;
          Instruction += "&" + AnsiString::IntToHex(ea, 2);
        }
				break;

			  case 'W':   /* Memory address word */
        {
          ea = Engine_ReadMem(NativeCPC, addr++);
          ea += Engine_ReadMem(NativeCPC, addr++) << 8;
          Instruction += "&" + AnsiString::IntToHex(ea, 4);
          MachineCode += AnsiString::IntToHex(ea & 0xff, 2) + " " + AnsiString::IntToHex((ea >> 8) & 0xff, 2) + " ";
        }
				break;

			  case 'X':
			  case 'Y':
        {
				  offset = (tChar)Engine_ReadMem(NativeCPC, addr++);
          Instruction += "(" + AnsiString(ixy);
          Instruction += (offset > 0) ? "+" : "-";
          Instruction += (offset > 0) ? AnsiString(offset) : AnsiString(-offset);
          Instruction += ")";
          MachineCode += AnsiString::IntToHex(offset & 0xff, 2) + " ";
        }
				break;

			  case 'I':
        {
          Instruction += AnsiString(ixy);
        }
				break;

			  case 'H':
        {
          tUChar byte = Engine_ReadMem(NativeCPC, NativeCPC->Z80->Regs.HL.w.l);
          MemoryContent = "(HL)=&" + AnsiString::IntToHex(byte, 2);
        }
				break;

        default:
        {
          Instruction += AnsiString(*src);
        }
      }
      
      src++;
    }
  }

  Instruction = Instruction.UpperCase();

  return addr;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugZ80Editor::PanelFormatRegAFClick(TObject *Sender)
{
  if (PanelFormatRegAF->Caption.IsEmpty() == true)
  {
    PanelFormatRegAF->Caption = "&&";
    tULong CurrentValue = PanelRegA->Caption.ToInt();
    PanelRegA->Caption = AnsiString::IntToHex(CurrentValue, 2);
    CurrentValue = PanelRegF->Caption.ToInt();
    PanelRegF->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegAF->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegA->Caption);
    PanelRegA->Caption = CurrentValue;
    CurrentValue = StrToInt("0x" + PanelRegF->Caption);
    PanelRegF->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegBCClick(TObject *Sender)
{
  if (PanelFormatRegBC->Caption.IsEmpty() == true)
  {
    PanelFormatRegBC->Caption = "&&";
    tULong CurrentValue = PanelRegB->Caption.ToInt();
    PanelRegB->Caption = AnsiString::IntToHex(CurrentValue, 2);
    CurrentValue = PanelRegC->Caption.ToInt();
    PanelRegC->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegBC->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegB->Caption);
    PanelRegB->Caption = CurrentValue;
    CurrentValue = StrToInt("0x" + PanelRegC->Caption);
    PanelRegC->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegDEClick(TObject *Sender)
{
  if (PanelFormatRegDE->Caption.IsEmpty() == true)
  {
    PanelFormatRegDE->Caption = "&&";
    tULong CurrentValue = PanelRegD->Caption.ToInt();
    PanelRegD->Caption = AnsiString::IntToHex(CurrentValue, 2);
    CurrentValue = PanelRegE->Caption.ToInt();
    PanelRegE->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegDE->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegD->Caption);
    PanelRegD->Caption = CurrentValue;
    CurrentValue = StrToInt("0x" + PanelRegE->Caption);
    PanelRegE->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegHLClick(TObject *Sender)
{
  if (PanelFormatRegHL->Caption.IsEmpty() == true)
  {
    PanelFormatRegHL->Caption = "&&";
    tULong CurrentValue = PanelRegH->Caption.ToInt();
    PanelRegH->Caption = AnsiString::IntToHex(CurrentValue, 2);
    CurrentValue = PanelRegL->Caption.ToInt();
    PanelRegL->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegHL->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegH->Caption);
    PanelRegH->Caption = CurrentValue;
    CurrentValue = StrToInt("0x" + PanelRegL->Caption);
    PanelRegL->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegAFAltClick(
      TObject *Sender)
{
  if (PanelFormatRegAFAlt->Caption.IsEmpty() == true)
  {
    PanelFormatRegAFAlt->Caption = "&&";
    tULong CurrentValue = PanelRegAAlt->Caption.ToInt();
    PanelRegAAlt->Caption = AnsiString::IntToHex(CurrentValue, 2);
    CurrentValue = PanelRegFAlt->Caption.ToInt();
    PanelRegFAlt->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegAFAlt->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegAAlt->Caption);
    PanelRegAAlt->Caption = CurrentValue;
    CurrentValue = StrToInt("0x" + PanelRegFAlt->Caption);
    PanelRegFAlt->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegBCAltClick(
      TObject *Sender)
{
  if (PanelFormatRegBCAlt->Caption.IsEmpty() == true)
  {
    PanelFormatRegBCAlt->Caption = "&&";
    tULong CurrentValue = PanelRegBAlt->Caption.ToInt();
    PanelRegBAlt->Caption = AnsiString::IntToHex(CurrentValue, 2);
    CurrentValue = PanelRegCAlt->Caption.ToInt();
    PanelRegCAlt->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegBCAlt->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegBAlt->Caption);
    PanelRegBAlt->Caption = CurrentValue;
    CurrentValue = StrToInt("0x" + PanelRegCAlt->Caption);
    PanelRegCAlt->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegDEAltClick(
      TObject *Sender)
{
  if (PanelFormatRegDEAlt->Caption.IsEmpty() == true)
  {
    PanelFormatRegDEAlt->Caption = "&&";
    tULong CurrentValue = PanelRegDAlt->Caption.ToInt();
    PanelRegDAlt->Caption = AnsiString::IntToHex(CurrentValue, 2);
    CurrentValue = PanelRegEAlt->Caption.ToInt();
    PanelRegEAlt->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegDEAlt->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegDAlt->Caption);
    PanelRegDAlt->Caption = CurrentValue;
    CurrentValue = StrToInt("0x" + PanelRegEAlt->Caption);
    PanelRegEAlt->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegHLAltClick(
      TObject *Sender)
{
  if (PanelFormatRegHLAlt->Caption.IsEmpty() == true)
  {
    PanelFormatRegHLAlt->Caption = "&&";
    tULong CurrentValue = PanelRegHAlt->Caption.ToInt();
    PanelRegHAlt->Caption = AnsiString::IntToHex(CurrentValue, 2);
    CurrentValue = PanelRegLAlt->Caption.ToInt();
    PanelRegLAlt->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegHLAlt->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegHAlt->Caption);
    PanelRegHAlt->Caption = CurrentValue;
    CurrentValue = StrToInt("0x" + PanelRegLAlt->Caption);
    PanelRegLAlt->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegIXClick(TObject *Sender)
{
  if (PanelFormatRegIX->Caption.IsEmpty() == true)
  {
    PanelFormatRegIX->Caption = "&&";
    tULong CurrentValue = PanelRegIX->Caption.ToInt();
    PanelRegIX->Caption = AnsiString::IntToHex(CurrentValue, 4);
  }
  else
  {
    PanelFormatRegIX->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegIX->Caption);
    PanelRegIX->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegIYClick(TObject *Sender)
{
  if (PanelFormatRegIY->Caption.IsEmpty() == true)
  {
    PanelFormatRegIY->Caption = "&&";
    tULong CurrentValue = PanelRegIY->Caption.ToInt();
    PanelRegIY->Caption = AnsiString::IntToHex(CurrentValue, 4);
  }
  else
  {
    PanelFormatRegIY->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegIY->Caption);
    PanelRegIY->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegSPClick(TObject *Sender)
{
  if (PanelFormatRegSP->Caption.IsEmpty() == true)
  {
    PanelFormatRegSP->Caption = "&&";
    tULong CurrentValue = PanelRegSP->Caption.ToInt();
    PanelRegSP->Caption = AnsiString::IntToHex(CurrentValue, 4);
  }
  else
  {
    PanelFormatRegSP->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegSP->Caption);
    PanelRegSP->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegPCClick(TObject *Sender)
{
  if (PanelFormatRegPC->Caption.IsEmpty() == true)
  {
    PanelFormatRegPC->Caption = "&&";
    tULong CurrentValue = PanelRegPC->Caption.ToInt();
    PanelRegPC->Caption = AnsiString::IntToHex(CurrentValue, 4);
  }
  else
  {
    PanelFormatRegPC->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegPC->Caption);
    PanelRegPC->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegIClick(TObject *Sender)
{
  if (PanelFormatRegI->Caption.IsEmpty() == true)
  {
    PanelFormatRegI->Caption = "&&";
    tULong CurrentValue = PanelRegI->Caption.ToInt();
    PanelRegI->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegI->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegI->Caption);
    PanelRegI->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelFormatRegRClick(TObject *Sender)
{
  if (PanelFormatRegR->Caption.IsEmpty() == true)
  {
    PanelFormatRegR->Caption = "&&";
    tULong CurrentValue = PanelRegR->Caption.ToInt();
    PanelRegR->Caption = AnsiString::IntToHex(CurrentValue, 2);
  }
  else
  {
    PanelFormatRegR->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelRegR->Caption);
    PanelRegR->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugZ80Editor::ComboBoxFlagChange(TObject *Sender)
{
TComboBox* ComboBox = (TComboBox*)Sender;
tUChar mask = (tUChar)ComboBox->Tag;

  // Update flags
  FormMain->NativeCPC->Z80->Regs.AF.b.l &= ~mask;
  if (ComboBox->ItemIndex == 1)
  {
    FormMain->NativeCPC->Z80->Regs.AF.b.l |= mask;
  }

  UpdateRegisters(FormMain->NativeCPC);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::ComboBoxFlag0DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxFlag0->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxFlag0->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxFlag0->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::ComboBoxFlag1DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxFlag1->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxFlag1->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxFlag1->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::ComboBoxFlag2DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxFlag2->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxFlag2->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxFlag2->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::ComboBoxFlag3DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxFlag3->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxFlag3->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxFlag3->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::ComboBoxFlag4DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxFlag4->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxFlag4->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxFlag4->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::ComboBoxFlag5DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxFlag5->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxFlag5->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxFlag5->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::ComboBoxFlag6DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxFlag6->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxFlag6->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxFlag6->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::ComboBoxFlag7DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxFlag7->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxFlag7->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxFlag7->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugZ80Editor::PanelRegAClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.AF.b.h;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegAF->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegA->Caption = (PanelFormatRegAF->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegFClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.AF.b.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegAF->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegF->Caption = (PanelFormatRegAF->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
    UpdateFlags(FormMain->NativeCPC);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegBClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.BC.b.h;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegBC->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegB->Caption = (PanelFormatRegBC->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegCClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.BC.b.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegBC->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegC->Caption = (PanelFormatRegBC->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegDClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.DE.b.h;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegDE->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegD->Caption = (PanelFormatRegDE->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegEClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.DE.b.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegDE->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegE->Caption = (PanelFormatRegDE->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegHClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.HL.b.h;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegHL->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegH->Caption = (PanelFormatRegHL->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegLClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.HL.b.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegHL->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegL->Caption = (PanelFormatRegHL->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegAAltClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.AFx.b.h;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegAFAlt->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegAAlt->Caption = (PanelFormatRegAFAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegFAltClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.AFx.b.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegAFAlt->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegFAlt->Caption = (PanelFormatRegAFAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegBAltClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.BCx.b.h;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegBCAlt->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegBAlt->Caption = (PanelFormatRegBCAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegCAltClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.BCx.b.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegBCAlt->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegCAlt->Caption = (PanelFormatRegBCAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegDAltClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.DEx.b.h;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegDEAlt->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegDAlt->Caption = (PanelFormatRegDEAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegEAltClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.DEx.b.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegDEAlt->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegEAlt->Caption = (PanelFormatRegDEAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegHAltClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.HLx.b.h;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegHLAlt->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegHAlt->Caption = (PanelFormatRegHLAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegLAltClick(TObject *Sender)
{
tUChar* Regs = &FormMain->NativeCPC->Z80->Regs.HLx.b.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = PanelFormatRegHLAlt->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegLAlt->Caption = (PanelFormatRegHLAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegIXClick(TObject *Sender)
{
tUShort* Regs = &FormMain->NativeCPC->Z80->Regs.IX.w.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUShort)*Regs;
  EditValue->Hexadecimal = PanelFormatRegIX->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 4;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 65535;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegIX->Caption = (PanelFormatRegIX->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 4) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegIYClick(TObject *Sender)
{
tUShort* Regs = &FormMain->NativeCPC->Z80->Regs.IY.w.l;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUShort)*Regs;
  EditValue->Hexadecimal = PanelFormatRegIY->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 4;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 65535;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegIY->Caption = (PanelFormatRegIY->Caption.IsEmpty() == false) ? AnsiString::IntToHex(*Regs, 4) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegSPClick(TObject *Sender)
{
tUShort* Regs = &FormMain->NativeCPC->Z80->Regs.SP.w.l;
bool ShowHexa = PanelFormatRegSP->Caption.IsEmpty() == true ? false : true;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUShort)*Regs;
  EditValue->Hexadecimal = ShowHexa;
  EditValue->NbDigits = 4;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 65535;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegSP->Caption = (ShowHexa == true) ? AnsiString::IntToHex(*Regs, 4) : AnsiString(*Regs);

    UpdateStack(FormMain->NativeCPC);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegPCClick(TObject *Sender)
{
tUShort* Regs = &FormMain->NativeCPC->Z80->Regs.PC.w.l;
bool ShowHexa = PanelFormatRegPC->Caption.IsEmpty() == true ? false : true;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUShort)*Regs;
  EditValue->Hexadecimal = ShowHexa;
  EditValue->NbDigits = 4;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 65535;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegPC->Caption = (ShowHexa == true) ? AnsiString::IntToHex(*Regs, 4) : AnsiString(*Regs);

    UpdateInstructions(FormMain->NativeCPC);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegIClick(TObject *Sender)
{
tULong* Regs = &FormMain->NativeCPC->Z80->Regs.I;
bool ShowHexa = PanelFormatRegI->Caption.IsEmpty() == true ? false : true;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = ShowHexa;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegI->Caption = (ShowHexa == true) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelRegRClick(TObject *Sender)
{
tULong* Regs = &FormMain->NativeCPC->Z80->Regs.R;
bool ShowHexa = PanelFormatRegR->Caption.IsEmpty() == true ? false : true;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (tUChar)*Regs;
  EditValue->Hexadecimal = ShowHexa;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    *Regs = EditValue->NewValue;
    PanelRegR->Caption = (ShowHexa == true) ? AnsiString::IntToHex(*Regs, 2) : AnsiString(*Regs);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugZ80Editor::ComboBoxITModeChange(TObject *Sender)
{
  // Update interrupt mode
  FormMain->NativeCPC->Z80->Regs.IM = ComboBoxITMode->ItemIndex;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugZ80Editor::ComboBoxITModeDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxITMode->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxITMode->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxITMode->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugZ80Editor::PanelFormatBreakpoint1Click(
      TObject *Sender)
{
  if (PanelFormatBreakpoint1->Caption.IsEmpty() == true)
  {
    PanelFormatBreakpoint1->Caption = "&&";
    tULong CurrentValue = PanelBreakpoint1->Caption.ToInt();
    PanelBreakpoint1->Caption = AnsiString::IntToHex(CurrentValue, 4);
  }
  else
  {
    PanelFormatBreakpoint1->Caption = "";
    tULong CurrentValue = StrToInt("0x" + PanelBreakpoint1->Caption);
    PanelBreakpoint1->Caption = CurrentValue;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::CheckBoxBreakpoint1Click(
      TObject *Sender)
{
  if (CheckBoxBreakpoint1->Focused())
  {
    tZ80* Z80 = FormMain->NativeCPC->Z80;

    if (CheckBoxBreakpoint1->Checked == true)
    {
      if (PanelFormatBreakpoint1->Caption.IsEmpty() == false)
      {
        Z80->Regs.breakpoint = StrToInt("0x" + PanelBreakpoint1->Caption);
      }
      else
      {
        Z80->Regs.breakpoint = PanelBreakpoint1->Caption.ToInt();
      }
    }
    else
    {
      Z80->Regs.breakpoint = 0xffffffff; // clear break point
    }
    mBreakpoint1 = Z80->Regs.breakpoint;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::PanelBreakpoint1Click(TObject *Sender)
{
bool ShowHexa = PanelFormatBreakpoint1->Caption.IsEmpty() == true ? false : true;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = (ShowHexa == true) ? StrToInt("0x" + PanelBreakpoint1->Caption) :
                                                   PanelBreakpoint1->Caption.ToInt();;
  EditValue->Hexadecimal = ShowHexa;
  EditValue->NbDigits = 4;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 65535;
  if (EditValue->ShowModal() == mrOk)
  {
    tZ80* Z80 = FormMain->NativeCPC->Z80;

    PanelBreakpoint1->Caption = (ShowHexa == true) ? AnsiString::IntToHex(EditValue->NewValue, 4) :
                                                     AnsiString(EditValue->NewValue);

    // Set new breakpoint
    CheckBoxBreakpoint1->Checked = true;
    if (PanelFormatBreakpoint1->Caption.IsEmpty() == false)
    {
      Z80->Regs.breakpoint = StrToInt("0x" + PanelBreakpoint1->Caption);
    }
    else
    {
      Z80->Regs.breakpoint = PanelBreakpoint1->Caption.ToInt();
    }
    mBreakpoint1 = Z80->Regs.breakpoint;
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugZ80Editor::UpdateRegisters(tNativeCPC* NativeCPC)
{
tZ80* Z80 = NativeCPC->Z80;

  PanelRegA->Caption = (PanelFormatRegAF->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_A, 2) : AnsiString(_A);
  PanelRegF->Caption = (PanelFormatRegAF->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_F, 2) : AnsiString(_F);
  PanelRegB->Caption = (PanelFormatRegBC->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_B, 2) : AnsiString(_B);
  PanelRegC->Caption = (PanelFormatRegBC->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_C, 2) : AnsiString(_C);
  PanelRegD->Caption = (PanelFormatRegDE->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_D, 2) : AnsiString(_D);
  PanelRegE->Caption = (PanelFormatRegDE->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_E, 2) : AnsiString(_E);
  PanelRegH->Caption = (PanelFormatRegHL->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_H, 2) : AnsiString(_H);
  PanelRegL->Caption = (PanelFormatRegHL->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_L, 2) : AnsiString(_L);

  PanelRegAAlt->Caption = (PanelFormatRegAFAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(Z80->Regs.AFx.b.h, 2) : AnsiString(Z80->Regs.AFx.b.h);
  PanelRegFAlt->Caption = (PanelFormatRegAFAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(Z80->Regs.AFx.b.l, 2) : AnsiString(Z80->Regs.AFx.b.l);
  PanelRegBAlt->Caption = (PanelFormatRegBCAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(Z80->Regs.BCx.b.h, 2) : AnsiString(Z80->Regs.BCx.b.h);
  PanelRegCAlt->Caption = (PanelFormatRegBCAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(Z80->Regs.BCx.b.l, 2) : AnsiString(Z80->Regs.BCx.b.l);
  PanelRegDAlt->Caption = (PanelFormatRegDEAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(Z80->Regs.DEx.b.h, 2) : AnsiString(Z80->Regs.DEx.b.h);
  PanelRegEAlt->Caption = (PanelFormatRegDEAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(Z80->Regs.DEx.b.l, 2) : AnsiString(Z80->Regs.DEx.b.l);
  PanelRegHAlt->Caption = (PanelFormatRegHLAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(Z80->Regs.HLx.b.h, 2) : AnsiString(Z80->Regs.HLx.b.h);
  PanelRegLAlt->Caption = (PanelFormatRegHLAlt->Caption.IsEmpty() == false) ? AnsiString::IntToHex(Z80->Regs.HLx.b.l, 2) : AnsiString(Z80->Regs.HLx.b.l);

  PanelRegIX->Caption = (PanelFormatRegIX->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_IX, 4) : AnsiString(_IX);
  PanelRegIY->Caption = (PanelFormatRegIY->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_IY, 4) : AnsiString(_IY);
  PanelRegSP->Caption = (PanelFormatRegSP->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_SP, 4) : AnsiString(_SP);
  PanelRegPC->Caption = (PanelFormatRegPC->Caption.IsEmpty() == false) ? AnsiString::IntToHex(_PC, 4) : AnsiString(_PC);

  PanelRegI->Caption = (PanelFormatRegI->Caption.IsEmpty() == false) ? AnsiString::IntToHex((tUChar)_I, 2) : AnsiString((tUChar)_I);
  PanelRegR->Caption = (PanelFormatRegR->Caption.IsEmpty() == false) ? AnsiString::IntToHex((tUChar)_R, 2) : AnsiString((tUChar)_R);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::UpdateFlags(tNativeCPC* NativeCPC)
{
tUChar flags = NativeCPC->Z80->Regs.AF.b.l;

  ComboBoxFlag0->ItemIndex = (flags & 0x01) ? 1 : 0;
  ComboBoxFlag1->ItemIndex = (flags & 0x02) ? 1 : 0;
  ComboBoxFlag2->ItemIndex = (flags & 0x04) ? 1 : 0;
  ComboBoxFlag3->ItemIndex = (flags & 0x08) ? 1 : 0;
  ComboBoxFlag4->ItemIndex = (flags & 0x10) ? 1 : 0;
  ComboBoxFlag5->ItemIndex = (flags & 0x20) ? 1 : 0;
  ComboBoxFlag6->ItemIndex = (flags & 0x40) ? 1 : 0;
  ComboBoxFlag7->ItemIndex = (flags & 0x80) ? 1 : 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::UpdateInstructions(tNativeCPC* NativeCPC)
{
AnsiString MachineCode;
AnsiString Instruction;
AnsiString MemoryContent;
tULong pc = NativeCPC->Z80->Regs.PC.w.l;
tULong addr = pc;
int IndexPC = 0;

  // Detect previous instructions
  for (int Loop=0; Loop < StringGridInstructions->RowCount; Loop++)
  {
    if (addr == mNextInstructions[Loop])
    {
      if (Loop > (StringGridInstructions->RowCount/2))
      {
        addr = mNextInstructions[Loop - StringGridInstructions->RowCount/2];
      }
      else
      {
        addr = mNextInstructions[0];
      }
    }
  }

  for (int Loop=0; Loop < StringGridInstructions->RowCount; Loop++)
  {
    bool ShowMemoryContent = false;
    StringGridInstructions->Cells[0][Loop] = AnsiString::IntToHex(addr,4);

    mNextInstructions[Loop] = addr;

    if (addr == pc)
    {
      IndexPC = Loop;
      ShowMemoryContent = true;
    }

    addr = GetInstruction(NativeCPC,
                          addr,
                          MachineCode,
                          Instruction,
                          MemoryContent);
    StringGridInstructions->Cells[1][Loop] = MachineCode;
    StringGridInstructions->Cells[2][Loop] = Instruction;
    if (ShowMemoryContent)
    {
      StringGridInstructions->Cells[3][Loop] = MemoryContent;
    }
    else
    {
      StringGridInstructions->Cells[3][Loop] = "";
    }
  }

  // Select instruction at PC
  TGridRect Rect;
  Rect.Top = Rect.Bottom = IndexPC;
  Rect.Left = 0;
  Rect.Right = 3;
  StringGridInstructions->Selection = Rect;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::UpdateStack(tNativeCPC* NativeCPC)
{
tULong addr = NativeCPC->Z80->Regs.SP.w.l;

  for (int Loop=0; Loop < StringGridStack->RowCount; Loop++)
  {
    StringGridStack->Cells[0][Loop] = AnsiString::IntToHex(addr,4);

    tUShort word = Engine_ReadMem(NativeCPC, addr++);
    word += Engine_ReadMem(NativeCPC, addr++) << 8;

    StringGridStack->Cells[1][Loop] = AnsiString::IntToHex(word, 4);
  }

  TGridRect Rect;
  Rect.Top = Rect.Bottom = 0;
  Rect.Left = 0;
  Rect.Right = 1;
  StringGridStack->Selection = Rect;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::UpdateInterrupt(tNativeCPC* NativeCPC)
{
tZ80* Z80 = NativeCPC->Z80;

  PanelIFF1->Caption = Z80->Regs.IFF1 ? "1" : "0";
  PanelIFF2->Caption = Z80->Regs.IFF2 ? "1" : "0";
  ComboBoxITMode->ItemIndex = Z80->Regs.IM & 0x03;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::InitBreakpoint(tNativeCPC* NativeCPC)
{
tZ80* Z80 = NativeCPC->Z80;

  //
  // Breakpoint 1
  //
  mBreakpoint1 = Z80->Regs.breakpoint;
  mBreakpoint1Highlighted = false;
  CheckBoxBreakpoint1->Checked = (Z80->Regs.breakpoint != 0xffffffff) ? true : false;
  if (CheckBoxBreakpoint1->Checked == true)
  {
    PanelBreakpoint1->Caption = (PanelFormatBreakpoint1->Caption.IsEmpty() == false) ? AnsiString::IntToHex(Z80->Regs.breakpoint, 4) :
                                                                          AnsiString(Z80->Regs.breakpoint);
  }
  else
  {
    PanelBreakpoint1->Caption = (PanelFormatBreakpoint1->Caption.IsEmpty() == false) ? "FFFF" : "65535";
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugZ80Editor::UpdateBreakpoint(tNativeCPC* NativeCPC)
{
tZ80* Z80 = NativeCPC->Z80;

	//
	// Breakpoint 1
	//
  if (mBreakpoint1Highlighted == false)
  {
    if (mBreakpoint1 == _PCdword)
    {
      PanelBreakpoint1->Color = clRed;
      PanelBreakpoint1->Font->Color = clWhite;
      mBreakpoint1Highlighted = true;
    }
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugZ80Editor::UpdateRegistersEdition(bool Allow)
{
	ComboBoxITMode->Enabled = Allow;

	PanelRegA->Enabled = Allow;
	PanelRegF->Enabled = Allow;
	PanelRegB->Enabled = Allow;
	PanelRegC->Enabled = Allow;
	PanelRegD->Enabled = Allow;
	PanelRegE->Enabled = Allow;
	PanelRegH->Enabled = Allow;
	PanelRegL->Enabled = Allow;

	PanelRegAAlt->Enabled = Allow;
	PanelRegFAlt->Enabled = Allow;
	PanelRegBAlt->Enabled = Allow;
	PanelRegCAlt->Enabled = Allow;
	PanelRegDAlt->Enabled = Allow;
	PanelRegEAlt->Enabled = Allow;
	PanelRegHAlt->Enabled = Allow;
	PanelRegLAlt->Enabled = Allow;

	PanelRegIX->Enabled = Allow;
	PanelRegIY->Enabled = Allow;
	PanelRegSP->Enabled = Allow;
	PanelRegPC->Enabled = Allow;

	PanelRegI->Enabled = Allow;
	PanelRegR->Enabled = Allow;

	ComboBoxFlag7->Enabled = Allow;
	ComboBoxFlag6->Enabled = Allow;
	ComboBoxFlag5->Enabled = Allow;
	ComboBoxFlag4->Enabled = Allow;
	ComboBoxFlag3->Enabled = Allow;
	ComboBoxFlag2->Enabled = Allow;
	ComboBoxFlag1->Enabled = Allow;
	ComboBoxFlag0->Enabled = Allow;
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugZ80Editor::OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;

  if (Condition & EC_BREAKPOINT)
  {
    UpdateBreakpoint(NativeCPC);
    Pause = true;
  }

  if ( (mPaused == true) && (Pause == false) )
  {
    mPaused = false;
		Caption = Title;

		// Prevent registers edition
		UpdateRegistersEdition(false);

    if (mBreakpoint1Highlighted == true)
    {
      PanelBreakpoint1->Color = clWhite;
      PanelBreakpoint1->Font->Color = clBlack;
      mBreakpoint1Highlighted = false;
    }
	}

  if ( ((Condition & EC_END_INSTRUCTION) == 0) && (Pause == false) )
    return false;

  if (mPaused == false)
  {
    mPaused = true;
    Caption = Title;

		// Allow registers edition
		UpdateRegistersEdition(true);

    UpdateRegisters(NativeCPC);
    UpdateFlags(NativeCPC);
    UpdateInstructions(NativeCPC);
    UpdateStack(NativeCPC);
    UpdateInterrupt(NativeCPC);
  }

  if (Condition & EC_END_INSTRUCTION)
  {
    if (mBreakpoint1Highlighted == true)
    {
      PanelBreakpoint1->Color = clWhite;
      PanelBreakpoint1->Font->Color = clBlack;
      mBreakpoint1Highlighted = false;
    }
  }

  if (Condition & (EC_END_INSTRUCTION | EC_BREAKPOINT | EC_FDC_EVENT) )
  {
    UpdateRegisters(NativeCPC);
    UpdateFlags(NativeCPC);
    UpdateInstructions(NativeCPC);
    UpdateStack(NativeCPC);
    UpdateInterrupt(NativeCPC);
  }

  return Pause;
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugZ80Editor::StringGridInstructionsDrawCell(TObject *Sender,
          int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
  TStringGrid* StringGrid = (TStringGrid*)Sender;

  if (State.Contains(gdSelected))
  {
    StringGrid->Canvas->Brush->Color = clHighlight;
    StringGrid->Canvas->Font->Color = clWhite;
  }
  else if (ACol == 1)
  {
    StringGrid->Canvas->Brush->Color = Color;
    StringGrid->Canvas->Font->Color = mSettingsP->ThemeText1Color;
  }
  else
  {
    StringGrid->Canvas->Brush->Color = Color;
    StringGrid->Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }

  // Draw cell
  StringGrid->Canvas->FillRect(Rect);
  int TextHeight = StringGrid->Canvas->TextHeight(StringGrid->Cells[ACol][ARow]);
  int TextPos = (StringGrid->RowHeights[ARow] - TextHeight) / 2;
  StringGrid->Canvas->TextOut(Rect.Left+5,
                              Rect.Top+TextPos,
                              StringGrid->Cells[ACol][ARow]);
  if (State.Contains(gdFocused))
  {
    StringGrid->Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

