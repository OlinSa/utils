#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "h264_wrap.h"

using namespace std;

unsigned int Ue(unsigned char *pBuff, unsigned int nLen, unsigned int &nStartBit)
{
	//计算0bit的个数
	unsigned int nZeroNum = 0;
	while (nStartBit < nLen * 8)
	{
		if (pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8))) //&:按位与，%取余
		{
			break;
		}
		nZeroNum++;
		nStartBit++;
	}
	nStartBit++;

	//计算结果
	unsigned long dwRet = 0;
	for (unsigned int i = 0; i < nZeroNum; i++)
	{

		dwRet <<= 1;
		if (pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8)))
		{
			dwRet += 1;
		}
		nStartBit++;
	}
	return (1 << nZeroNum) - 1 + dwRet;
}

int Se(unsigned char *pBuff, unsigned int nLen, unsigned int &nStartBit)
{
	int UeVal = Ue(pBuff, nLen, nStartBit);
	double k = UeVal;
	int nValue = ceil(k / 2); //ceil函数：ceil函数的作用是求不小于给定实数的最小整数。ceil(2)=ceil(1.2)=cei(1.5)=2.00
	if (UeVal % 2 == 0)
		nValue = -nValue;
	return nValue;
}

unsigned long u(unsigned int BitCount, unsigned char *buf, unsigned int &nStartBit)
{
	unsigned long dwRet = 0;
	for (unsigned int i = 0; i < BitCount; i++)
	{
		dwRet <<= 1;
		if (buf[nStartBit / 8] & (0x80 >> (nStartBit % 8)))
		{
			dwRet += 1;
		}
		nStartBit++;
	}
	return dwRet;
}

int H264Wrap::GetNaluFromBuffer(NaluUnit &nalu, unsigned char *buf, int bufSize)
{
	int i = 0;
	int startCodeCnt = 0;
	unsigned char *pbuf = buf;
	while (i < bufSize)
	{
		if (pbuf[i++] == 0x00 && pbuf[i++] == 0x00)
		{
			if (pbuf[i] == 0x01)
			{
				i += 1;
				startCodeCnt = 3;
			}
			else if (pbuf[i] == 0x00 && pbuf[i + 1] == 0x01)
			{
				i += 2;
				startCodeCnt = 4;
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		//find next frame
		int pos = i;
		int startCodeCnt2 = 0;

		while (i < bufSize)
		{
			if (pbuf[i++] == 0x00 && pbuf[i++] == 0x00)
			{
				if (pbuf[i] == 0x01)
				{
					i += 1;
					startCodeCnt2 = 3;
					break;
				}
				else if (pbuf[i] == 0x00 && pbuf[i + 1] == 0x01)
				{
					i += 2;
					startCodeCnt2 = 4;
					break;
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}

		nalu.startCode = startCodeCnt;
		if (i > bufSize)
		{
			return -1;
		}
		else if (i == bufSize)
		{
			nalu.size = i - pos;
		}
		else
		{
			nalu.size = (i - startCodeCnt2) - pos;
		}
		nalu.type = pbuf[pos] & 0x1f;
		nalu.data = &pbuf[pos];
		return i - startCodeCnt2;
	}
	return -1;
}

void H264Wrap::PrintH264Info(std::string filename)
{
	unsigned char buf[10240];
	int pos = 0;
	int alreadSeen = 0;
	int bufSize = sizeof(buf);

	int nextFramePos;
	std::streamsize readCount;
	int naluCount = 0;

	NaluUnit nalu;

	ifstream inFile(filename, ios::in | ios::binary);
	if (!inFile)
	{
		LOG_DEBUG("open %s failed", filename);
		return;
	}
	while (inFile.read(reinterpret_cast<char *>(&buf[0]) + alreadSeen, bufSize - alreadSeen))
	{
		readCount = inFile.gcount();
		alreadSeen += readCount;

		while ((nextFramePos = GetNaluFromBuffer(nalu, buf + pos, alreadSeen - pos)) > 0)
		{
			naluCount++;
			LOG_DEBUG("%d nalu: startcode:%d,type:%d, data:%p, dataSize:%d,nextFramePos:%d",
					  naluCount, nalu.startCode, nalu.type, nalu.data, nalu.size, nextFramePos);
			pos += nextFramePos;
			//                pos          alreadSeen      bufSize
			// |  #############| ############| #############|

			// for (int i = 0; i < 38; i++)
			// {
			//     if (i % 8 == 0)
			//     {
			//         cout << endl;
			//     }
			//     cout << "0x" << setfill('0') << setw(2) << hex << (buf[i] & 0xff) << " ";
			// }
			// cout << endl;
			// sleep(1);
		}
		if (pos > 0)
		{
			memmove(buf, buf + pos, alreadSeen - pos); //memcpy会有内存重叠问题
			alreadSeen -= pos;
			pos = 0;
		}
		else
		{
			pos = 0;
			alreadSeen = 0;
		}
	}
	inFile.close();
	return;
}

bool H264Wrap::DecodeSPS(unsigned char *data, int dataLen, int &width, int &height, int &fps)
{
	KickOutNaluRaceCondition(data, dataLen);
	unsigned int startBits = 0;
	int forbit_zero_bit = u(1, data, startBits);
	int nal_ref_idc = u(2, data, startBits);
	int nal_unit_type = u(5, data, startBits);
	fps = 0;

	if (nal_unit_type == NALU_TYPE_SPS)
	{
		int profile_idc = u(8, data, startBits);
		int constraint_set0_flag = u(1, data, startBits); //(buf[1] & 0x80)>>7;
		int constraint_set1_flag = u(1, data, startBits); //(buf[1] & 0x40)>>6;
		int constraint_set2_flag = u(1, data, startBits); //(buf[1] & 0x20)>>5;
		int constraint_set3_flag = u(1, data, startBits); //(buf[1] & 0x10)>>4;
		int reserved_zero_4bits = u(4, data, startBits);
		int level_idc = u(8, data, startBits);

		int seq_parameter_set_id = Ue(data, dataLen, startBits);
		if (profile_idc == 100 || profile_idc == 110 ||
			profile_idc == 122 || profile_idc == 144)
		{
			int chroma_format_idc = Ue(data, dataLen, startBits);
			if (chroma_format_idc == 3)
				int residual_colour_transform_flag = u(1, data, startBits);
			int bit_depth_luma_minus8 = Ue(data, dataLen, startBits);
			int bit_depth_chroma_minus8 = Ue(data, dataLen, startBits);
			int qpprime_y_zero_transform_bypass_flag = u(1, data, startBits);
			int seq_scaling_matrix_present_flag = u(1, data, startBits);

			int seq_scaling_list_present_flag[8];
			if (seq_scaling_matrix_present_flag)
			{
				for (int i = 0; i < 8; i++)
				{
					seq_scaling_list_present_flag[i] = u(1, data, startBits);
				}
			}
		}
		int log2_max_frame_num_minus4 = Ue(data, dataLen, startBits);
		int pic_order_cnt_type = Ue(data, dataLen, startBits);
		if (pic_order_cnt_type == 0)
			int log2_max_pic_order_cnt_lsb_minus4 = Ue(data, dataLen, startBits);
		else if (pic_order_cnt_type == 1)
		{
			int delta_pic_order_always_zero_flag = u(1, data, startBits);
			int offset_for_non_ref_pic = Se(data, dataLen, startBits);
			int offset_for_top_to_bottom_field = Se(data, dataLen, startBits);
			int num_ref_frames_in_pic_order_cnt_cycle = Ue(data, dataLen, startBits);

			int *offset_for_ref_frame = new int[num_ref_frames_in_pic_order_cnt_cycle];
			for (int i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++)
				offset_for_ref_frame[i] = Se(data, dataLen, startBits);
			delete[] offset_for_ref_frame;
		}
		int num_ref_frames = Ue(data, dataLen, startBits);
		int gaps_in_frame_num_value_allowed_flag = u(1, data, startBits);
		int pic_width_in_mbs_minus1 = Ue(data, dataLen, startBits);
		int pic_height_in_map_units_minus1 = Ue(data, dataLen, startBits);

		width = (pic_width_in_mbs_minus1 + 1) * 16;
		height = (pic_height_in_map_units_minus1 + 1) * 16;

		int frame_mbs_only_flag = u(1, data, startBits);
		if (!frame_mbs_only_flag)
			int mb_adaptive_frame_field_flag = u(1, data, startBits);

		int direct_8x8_inference_flag = u(1, data, startBits);
		int frame_cropping_flag = u(1, data, startBits);
		if (frame_cropping_flag)
		{
			int frame_crop_left_offset = Ue(data, dataLen, startBits);
			int frame_crop_right_offset = Ue(data, dataLen, startBits);
			int frame_crop_top_offset = Ue(data, dataLen, startBits);
			int frame_crop_bottom_offset = Ue(data, dataLen, startBits);
		}
		int vui_parameter_present_flag = u(1, data, startBits);
		if (vui_parameter_present_flag)
		{
			int aspect_ratio_info_present_flag = u(1, data, startBits);
			if (aspect_ratio_info_present_flag)
			{
				int aspect_ratio_idc = u(8, data, startBits);
				if (aspect_ratio_idc == 255)
				{
					int sar_width = u(16, data, startBits);
					int sar_height = u(16, data, startBits);
				}
			}
			int overscan_info_present_flag = u(1, data, startBits);
			if (overscan_info_present_flag)
				int overscan_appropriate_flagu = u(1, data, startBits);
			int video_signal_type_present_flag = u(1, data, startBits);
			if (video_signal_type_present_flag)
			{
				int video_format = u(3, data, startBits);
				int video_full_range_flag = u(1, data, startBits);
				int colour_description_present_flag = u(1, data, startBits);
				if (colour_description_present_flag)
				{
					int colour_primaries = u(8, data, startBits);
					int transfer_characteristics = u(8, data, startBits);
					int matrix_coefficients = u(8, data, startBits);
				}
			}
			int chroma_loc_info_present_flag = u(1, data, startBits);
			if (chroma_loc_info_present_flag)
			{
				int chroma_sample_loc_type_top_field = Ue(data, dataLen, startBits);
				int chroma_sample_loc_type_bottom_field = Ue(data, dataLen, startBits);
			}
			int timing_info_present_flag = u(1, data, startBits);
			if (timing_info_present_flag)
			{
				int num_units_in_tick = u(32, data, startBits);
				int time_scale = u(32, data, startBits);
				fps = time_scale / (2 * num_units_in_tick);
			}
		}
		return true;
	}
	else
		return false;
}

void H264Wrap::KickOutNaluRaceCondition(unsigned char *data, int &dataLen)
{
	int i = 0, j = 0;
	unsigned char *tmpPtr = data;
	int tmpBufSize = dataLen;
	int val = 0;

	for (i = 0; i < (tmpBufSize - 2); i++)
	{
		//check for 0x000003
		val = (tmpPtr[i] ^ 0x00) + (tmpPtr[i + 1] ^ 0x00) + (tmpPtr[i + 2] ^ 0x03);
		if (val == 0)
		{
			//kick out 0x03
			for (j = i + 2; j < tmpBufSize - 1; j++)
				tmpPtr[j] = tmpPtr[j + 1];

			//and so we should devrease bufsize
			dataLen--;
		}
	}

	return;
}