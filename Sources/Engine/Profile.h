/*
    CaPriCe for Palm OS - Amstrad CPC 464/664/6128 emulator for Palm devices
    Copyright (C) 2006-2011 by Frédéric Coste

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

#ifndef PROFILE_H
#define PROFILE_H

#ifdef _PROFILE

// List of functions
enum
{
  // 0
  PROFILE_CPCExecute_Main_Loop  = 0,
  PROFILE_z80_pfx_cb,
  PROFILE_z80_pfx_dd,
  PROFILE_z80_pfx_ddcb,
  PROFILE_z80_pfx_ed,
  PROFILE_z80_pfx_fd,
  PROFILE_z80_pfx_fdcb,
  PROFILE_z80_IN_handler,
  PROFILE_z80_OUT_handler,
  PROFILE_ga_memory_manager,
  // 10
  PROFILE_ga_init_banking,
  PROFILE_Z80_WAIT_STATES,
  PROFILE_read_mem,
  PROFILE_write_mem,
  PROFILE_get_byte_address,
  PROFILE_bad_mem_address,
  PROFILE_bad_reg_content,
  PROFILE_PSG_WRITE,
  PROFILE_video_access_memory_Loop,
  PROFILE_video_draw_border,
  // 20
  PROFILE_video_draw,
  PROFILE_audio_Synthesizer_Stereo16,
  PROFILE_audio_Synthesizer_Stereo16_Loop,
  PROFILE_audio_Synthesizer_Stereo8,
  PROFILE_audio_Synthesizer_Stereo8_Loop,
  PROFILE_fdc_read_data,
  PROFILE_fdc_write_data,
  PROFILE_fdc_read_status,
  PROFILE_fdc_overrun,
  PROFILE_HandleSpecialEvent,
  // 30
  PROFILE_SysHandleEvent,
  PROFILE_MenuHandleEvent,
  PROFILE_AppHandleEvent,
  PROFILE_FrmDispatchEvent,
  PROFILE_audio_Synthesizer_Mono8,
  PROFILE_audio_Synthesizer_Mono8_Loop,
  PROFILE_audio_set_AY_Register_13,
  PROFILE_audio_set_AY_Register_1_3_5,
  PROFILE_audio_set_AY_Register_6,
  PROFILE_audio_set_AY_Register_7,
  // 40
  PROFILE_audio_set_AY_Register_8,
  PROFILE_audio_set_AY_Register_9,
  PROFILE_audio_set_AY_Register_10,
  PROFILE_audio_set_AY_Register_0_2_4_11_12,
  PROFILE_Error_z80_IN_handler_1,
  PROFILE_Error_z80_IN_handler_CRTC_1,
  PROFILE_Error_z80_IN_handler_PPI_1,
  PROFILE_Error_z80_OUT_handler_1,
  PROFILE_Error_z80_OUT_handler_2,
  PROFILE_Error_z80_OUT_handler_3,
  // 50
  PROFILE_Error_z80_OUT_handler_GA_1,
  PROFILE_Error_z80_OUT_handler_CRTC_1,
  PROFILE_Error_z80_OUT_handler_CRTC_2,
  PROFILE_Error_z80_OUT_handler_FDC_1,
  PROFILE_Error_ga_memory_manager_1,
  PROFILE_Error_fdc_unknown_command,
  PROFILE_Error_fdc_bad_track_size,
  PROFILE_Error_Z80_INT_HANDLER_IM0,
  PROFILE_video_access_memory_1,
  PROFILE_video_access_memory_2,
  // 60
  PROFILE_video_access_memory_3,
  PROFILE_video_access_memory_4,
  PROFILE_video_access_memory_5,
  PROFILE_video_access_memory_6,
  PROFILE_video_access_memory_7,
  PROFILE_video_access_memory_8,
  PROFILE_video_access_memory_9,
  PROFILE_video_access_memory_10,
  PROFILE_video_access_memory_11,
  PROFILE_video_access_memory_12,
  // 70
  PROFILE_video_access_memory_13,
  PROFILE_video_access_memory_14,
  PROFILE_video_access_memory_15,
  PROFILE_video_access_memory_16,
  PROFILE_video_access_memory_17,
  PROFILE_video_access_memory_18,
  PROFILE_video_access_memory_19,
  PROFILE_video_access_memory_20,
  PROFILE_video_access_memory_21,
  PROFILE_video_access_memory_22,
  // 80
  PROFILE_video_access_memory_23,
  PROFILE_video_access_memory_24,
  PROFILE_video_access_memory_25,
  PROFILE_video_access_memory_26,
  PROFILE_video_access_memory_27,
  PROFILE_video_access_memory_28,
  PROFILE_video_access_memory_29,
  PROFILE_video_access_memory_30,
  PROFILE_video_access_memory_31,
  PROFILE_video_access_memory_32,
  // 90
  PROFILE_video_access_memory_33,

  // MUST be the last one
  PROFILE_NB_ITEMS
};
//

#if defined(__WIN32__)
char profileLabel[][50] = 
{
  // 0
  "CPCExecute_Main_Loop",
  "z80_pfx_cb",
  "z80_pfx_dd",
  "z80_pfx_ddcb",
  "z80_pfx_ed",
  "z80_pfx_fd",
  "z80_pfx_fdcb",
  "z80_IN_handler",
  "z80_OUT_handler",
  "ga_memory_manager",
  // 10
  "ga_init_banking",
  "Z80_WAIT_STATES",
  "read_mem",
  "write_mem",
  "get_byte_address",
  "bad_mem_address",
  "bad_reg_content",
  "PSG_WRITE",
  "video_access_memory_Loop",
  "video_draw_border",
  // 20
  "video_draw",
  "audio_Synthesizer_Stereo16",
  "audio_Synthesizer_Stereo16_Loop",
  "audio_Synthesizer_Stereo8",
  "audio_Synthesizer_Stereo8_Loop",
  "fdc_read_data",
  "fdc_write_data",
  "fdc_read_status",
  "fdc_overrun",
  "HandleSpecialEvent",
  // 30
  "SysHandleEvent",
  "MenuHandleEvent",
  "AppHandleEvent",
  "FrmDispatchEvent",
  "audio_Synthesizer_Mono8",
  "audio_Synthesizer_Mono8_Loop",
  "audio_set_AY_Register_13",
  "audio_set_AY_Register_1_3_5",
  "audio_set_AY_Register_6",
  "audio_set_AY_Register_7",
  // 40
  "audio_set_AY_Register_8",
  "audio_set_AY_Register_9",
  "audio_set_AY_Register_10",
  "audio_set_AY_Register_0_2_4_11_12",
  "Error_z80_IN_handler_1",
  "Error_z80_IN_handler_CRTC_1",
  "Error_z80_IN_handler_PPI_1",
  "Error_z80_OUT_handler_1",
  "Error_z80_OUT_handler_2",
  "Error_z80_OUT_handler_3",
  // 50
  "Error_z80_OUT_handler_GA_1",
  "Error_z80_OUT_handler_CRTC_1",
  "Error_z80_OUT_handler_CRTC_2",
  "Error_z80_OUT_handler_FDC_1",
  "Error_ga_memory_manager_1",
  "Error_fdc_unknown_command",
  "Error_fdc_bad_track_size",
  "Error_Z80_INT_HANDLER_IM0",
  "video_access_memory_1",
  "video_access_memory_2",
  // 60
  "video_access_memory_3",
  "video_access_memory_4",
  "video_access_memory_5",
  "video_access_memory_6",
  "video_access_memory_7",
  "video_access_memory_8",
  "video_access_memory_9",
  "video_access_memory_10",
  "video_access_memory_11",
  "video_access_memory_12",
  // 70
  "video_access_memory_13",
  "video_access_memory_14",
  "video_access_memory_15",
  "video_access_memory_16",
  "video_access_memory_17",
  "video_access_memory_18",
  "video_access_memory_19",
  "video_access_memory_20",
  "video_access_memory_21",
  "video_access_memory_22",
  // 80
  "video_access_memory_23",
  "video_access_memory_24",
  "video_access_memory_25",
  "video_access_memory_26",
  "video_access_memory_27",
  "video_access_memory_28",
  "video_access_memory_29",
  "video_access_memory_30",
  "video_access_memory_31",
  "video_access_memory_32",
  // 90
  "video_access_memory_33",
};
#endif /* __WIN32__ */

#endif /* _PROFILE */

#endif /* PROFILE_H */
