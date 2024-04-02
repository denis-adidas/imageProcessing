#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Windows.h";
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdint>
#define vvi vector<vector<int>>
#define vvd vector<vector<double>>
#define vvRGB vector<vector< RGBTRIPLE >>
#define vvYCbCr vector<vector< YCbCr >>
#define byte uint8_t
using namespace std;

struct YCbCr {
	double Y;
	double Cb;
	double Cr;
};

class Picture {
private:
	BITMAPFILEHEADER file_header;
	BITMAPINFOHEADER info_header;
	vector<vector< RGBTRIPLE >> vec;
	vector<vector<YCbCr>> vecYCbCr;

	void read(const char* file) {
		vec.clear();
		FILE* f = fopen(file, "r+b");
		RGBTRIPLE rgb_l;

		fread(&file_header, sizeof(file_header), 1, f);
		fread(&info_header, sizeof(info_header), 1, f);

		size_t padding = 0;
		if ((info_header.biWidth * 3) % 4)
			padding = 4 - (info_header.biWidth * 3) % 4;

		for (int i = 0; i < info_header.biHeight; i++) {
			vector<RGBTRIPLE> vecRow(static_cast<size_t>(info_header.biWidth));
			for (int j = 0; j < info_header.biWidth; j++) {
				fread(&rgb_l, sizeof(rgb_l), 1, f);
				vecRow[j] = rgb_l;
			}
			fseek(f, padding, SEEK_CUR);
			vec.push_back(vecRow);

			vecRow.clear();
		}
		fclose(f);
	}

	void write(const char* file, char flag = 's') {
		FILE* f = fopen(file, "w+b");
		RGBTRIPLE rgb;

		fwrite(&file_header, sizeof(file_header), 1, f);
		fwrite(&info_header, sizeof(info_header), 1, f);

		for (int j = 0; j < info_header.biHeight; j++) {
			for (int i = 0; i < info_header.biWidth; i++) {
				if (flag == 's')
					fwrite(&vec[j][i], sizeof(rgb), 1, f);
				if (flag == 'r') {
					RGBTRIPLE r = vec[j][i];
					r.rgbtGreen = 0;
					r.rgbtBlue = 0;
					fwrite(&r, sizeof(rgb), 1, f);
				}
				if (flag == 'g') {
					RGBTRIPLE g = vec[j][i];
					g.rgbtRed = 0;
					g.rgbtBlue = 0;
					fwrite(&g, sizeof(rgb), 1, f);
				}
				if (flag == 'b') {
					RGBTRIPLE b = vec[j][i];
					b.rgbtRed = 0;
					b.rgbtGreen = 0;
					fwrite(&b, sizeof(rgb), 1, f);
				}
			}
			size_t padding = 0;
			if ((info_header.biWidth * 3) % 4)
				padding = 4 - (info_header.biWidth * 3) % 4;
			if (padding != 0) {
				//fread(&rgb, padding, 1, f1);
				fwrite(&rgb, padding, 1, f);
			}
		}
		fclose(f);
	}

	double getCorrelation(char first, char second, int y = 0, int x = 0) {
		vector<vector<uint8_t>> firstColour;
		vector<vector<uint8_t>> SecondColour;
		int H = info_header.biHeight;
		int W = info_header.biWidth;
		if (y < 0) {
			for (int i = 0; i < H + y; i++) {
				vector<uint8_t> tmpSecond;
				int tpp = 0;
				int Wl = 0;
				if (x < 0) {
					Wl = W + x;
					tpp = 0;
				}
				else {
					tpp = x;
					Wl = W;
				}
				for (int j = tpp; j < Wl; j++) {
					if (second == 'r')
						tmpSecond.push_back(vec[i][j].rgbtRed);
					else if (second == 'g')
						tmpSecond.push_back(vec[i][j].rgbtGreen);
					else
						tmpSecond.push_back(vec[i][j].rgbtBlue);
				}
				SecondColour.push_back(tmpSecond);
			}
			for (int i = -y; i < H; i++) {
				vector<uint8_t> tmpFirst;
				int tpp = 0;
				int Wl = 0;
				if (x < 0) {
					Wl = W;
					tpp = -x;
				}
				else {
					tpp = 0;
					Wl = W - x;
				}
				for (int j = tpp; j < Wl; j++) {
					if (first == 'r')
						tmpFirst.push_back(vec[i][j].rgbtRed);
					else if (first == 'g')
						tmpFirst.push_back(vec[i][j].rgbtGreen);
					else
						tmpFirst.push_back(vec[i][j].rgbtBlue);
				}
				firstColour.push_back(tmpFirst);
			}
		}
		else {
			for (int i = 0; i < H - y; i++) {
				vector<uint8_t> tmpFirst;
				int tpp = 0;
				int Wl = 0;
				if (x < 0) {
					Wl = W;
					tpp = -x;
				}
				else {
					tpp = 0;
					Wl = W - x;
				}
				for (int j = tpp; j < Wl; j++) {
					if (first == 'r')
						tmpFirst.push_back(vec[i][j].rgbtRed);
					else if (first == 'g')
						tmpFirst.push_back(vec[i][j].rgbtGreen);
					else
						tmpFirst.push_back(vec[i][j].rgbtBlue);
				}
				firstColour.push_back(tmpFirst);
			}

			for (int i = y; i < H; i++) {
				vector<uint8_t> tmpSecond;
				int tpp = 0;
				int Wl = 0;
				if (x < 0) {
					Wl = W + x;
					tpp = 0;
				}
				else {
					tpp = x;
					Wl = W;
				}
				for (int j = tpp; j < Wl; j++) {
					if (second == 'r')
						tmpSecond.push_back(vec[i][j].rgbtRed);
					else if (second == 'g')
						tmpSecond.push_back(vec[i][j].rgbtGreen);
					else
						tmpSecond.push_back(vec[i][j].rgbtBlue);
				}
				SecondColour.push_back(tmpSecond);
			}
		}

		double mathExpFirst = 0;
		double mathExpSecond = 0;
		for (int i = 0; i < firstColour.size(); i++) {
			for (int j = 0; j < firstColour[i].size(); j++)
				mathExpFirst += firstColour[i][j];
		}
		for (int i = 0; i < SecondColour.size(); i++) {
			for (int j = 0; j < SecondColour[i].size(); j++)
				mathExpSecond += SecondColour[i][j];
		}
		mathExpFirst /= (double)(firstColour.size()) * (firstColour[0].size());
		mathExpSecond /= (double)(SecondColour.size()) * (SecondColour[0].size());

		double standDeviationFirst = 0;
		double standDeviationSecond = 0;

		for (int i = 0; i < firstColour.size(); i++) {
			for (int j = 0; j < firstColour[i].size(); j++)
				standDeviationFirst += pow(firstColour[i][j] - mathExpFirst, 2);
		}

		for (int i = 0; i < SecondColour.size(); i++)
			for (int j = 0; j < SecondColour[i].size(); j++)
				standDeviationSecond += pow(SecondColour[i][j] - mathExpSecond, 2);

		standDeviationFirst /= (double)(firstColour.size()) * (firstColour[0].size()) - 1;
		standDeviationSecond /= (double)(SecondColour.size()) * (SecondColour[0].size()) - 1;

		standDeviationFirst = sqrt(standDeviationFirst);
		standDeviationSecond = sqrt(standDeviationSecond);

		double rFS = 0;

		for (int i = 0; i < firstColour.size(); i++)
			for (int j = 0; j < firstColour[i].size(); j++)
				rFS += (firstColour[i][j] - mathExpFirst) * (SecondColour[i][j] - mathExpSecond);

		rFS /= (double)firstColour.size() * firstColour[0].size();
		rFS /= (double)standDeviationFirst * standDeviationSecond;
		return rFS;
	}

	double getCorrelationYCbCr(char first, char second) {
		vector<vector<uint8_t>> firstColour;
		vector<vector<uint8_t>> SecondColour;
		int H = info_header.biHeight;
		int W = info_header.biWidth;
		for (int i = 0; i < H; i++) {
			vector<uint8_t> tmpFirst;
			vector<uint8_t> tmpSecond;
			for (int j = 0; j < W; j++) {
				if (first == 'Y')
					tmpFirst.push_back(vecYCbCr[i][j].Y);
				else if (first == 'B')
					tmpFirst.push_back(vecYCbCr[i][j].Cb);
				else if (first == 'R')
					tmpFirst.push_back(vecYCbCr[i][j].Cr);
				if (second == 'Y')
					tmpSecond.push_back(vecYCbCr[i][j].Y);
				else if (second == 'B')
					tmpSecond.push_back(vecYCbCr[i][j].Cb);
				else if (second == 'R')
					tmpSecond.push_back(vecYCbCr[i][j].Cr);
			}
			firstColour.push_back(tmpFirst);
			SecondColour.push_back(tmpSecond);
		}

		double mathExpFirst = 0;
		double mathExpSecond = 0;
		for (int i = 0; i < firstColour.size(); i++) {
			for (int j = 0; j < firstColour[i].size(); j++)
				mathExpFirst += firstColour[i][j];
		}
		for (int i = 0; i < SecondColour.size(); i++) {
			for (int j = 0; j < SecondColour[i].size(); j++)
				mathExpSecond += SecondColour[i][j];
		}
		mathExpFirst /= (double)(firstColour.size()) * (firstColour[0].size());
		mathExpSecond /= (double)(SecondColour.size()) * (SecondColour[0].size());

		double standDeviationFirst = 0;
		double standDeviationSecond = 0;

		for (int i = 0; i < firstColour.size(); i++) {
			for (int j = 0; j < firstColour[i].size(); j++)
				standDeviationFirst += pow(firstColour[i][j] - mathExpFirst, 2);
		}

		for (int i = 0; i < SecondColour.size(); i++)
			for (int j = 0; j < SecondColour[i].size(); j++)
				standDeviationSecond += pow(SecondColour[i][j] - mathExpSecond, 2);

		standDeviationFirst /= (double)(firstColour.size()) * (firstColour[0].size()) - 1;
		standDeviationSecond /= (double)(SecondColour.size()) * (SecondColour[0].size()) - 1;

		standDeviationFirst = sqrt(standDeviationFirst);
		standDeviationSecond = sqrt(standDeviationSecond);

		double rFS = 0;

		for (int i = 0; i < firstColour.size(); i++)
			for (int j = 0; j < firstColour[i].size(); j++)
				rFS += (firstColour[i][j] - mathExpFirst) * (SecondColour[i][j] - mathExpSecond);

		rFS /= (double)firstColour.size() * firstColour[0].size();
		rFS /= (double)standDeviationFirst * standDeviationSecond;
		return rFS;
	}
	void writeGist(const char* file, vvi Dr, vvi Dg, vvi Db, vvd Dy, vvd Dcb, vvd Dcr, int k) {
		int tmpR[512], tmpG[512], tmpB[512], tmpCb[512], tmpCr[512], tmpY[512];

		for (int k = 0; k < 512; k++)
			tmpR[k] = tmpG[k] = tmpB[k] = tmpY[k] = tmpCb[k] = tmpCr[k] = 0;

		for (int i = 0; i < vec.size() - 1; i++) {
			for (int j = 0; j < vec[0].size() - 1; j++) {
				tmpR[(int)Dr[i][j] + 255] += 1;
				tmpG[(int)Dg[i][j] + 255] += 1;
				tmpB[(int)Db[i][j] + 255] += 1;
				tmpY[(int)Dy[i][j] + 255] += 1;
				tmpCr[(int)Dcb[i][j] + 255] += 1;
				tmpCb[(int)Dcr[i][j] + 255] += 1;
			}
		}

		ofstream out;
		out.open(file);
		for (int k = 0; k < 512; k++)
			out << k << " " << tmpR[k] << " " << tmpG[k] << " " << tmpB[k] << " " << tmpCr[k] << " " << tmpCb[k] << " " << tmpY[k] << "\n";
		out.close();
		double hr = 0, pr = 0, hg = 0, pg = 0, hb = 0, pb = 0, pCr = 0, pCb = 0, hCr = 0, hCb = 0, pY = 0, hY = 0;
		for (int i = 0; i < 512; i++) {
			pr = (double)tmpR[i] / ((double)info_header.biWidth * info_header.biHeight);
			pg = (double)tmpG[i] / ((double)info_header.biWidth * info_header.biHeight);
			pb = (double)tmpB[i] / ((double)info_header.biWidth * info_header.biHeight);
			pY = (double)tmpY[i] / ((double)info_header.biWidth * info_header.biHeight);
			pCr = (double)tmpCr[i] / ((double)info_header.biWidth * info_header.biHeight);
			pCb = (double)tmpCb[i] / ((double)info_header.biWidth * info_header.biHeight);
			if (pr > 0)
				hr += pr * log2(pr);
			if (pg > 0)
				hg += pg * log2(pg);
			if (pb > 0)
				hb += pb * log2(pb);
			if (pY > 0)
				hY += pY * log2(pY);
			if (pCr > 0)
				hCr += pCr * log2(pCr);
			if (pCb > 0)
				hCb += pCb * log2(pCb);
		}
		cout << "Entropy for Dr rule " << k << " :   " << -hr << "\n";
		cout << "Entropy for Dg rule " << k << " : " << -hg << "\n";
		cout << "Entropy for Db rule " << k << " :  " << -hb << "\n";
		cout << "Entropy for DY rule " << k << " :     " << -hY << "\n";
		cout << "Entropy for DCb rule " << k << " :    " << -hCb << "\n";
		cout << "Entropy for DCr rule " << k << " :    " << -hCr << "\n";
		cout << "\n\n";
	}

public:
	Picture() {	}

	//п.2 Считать файл
	void readPict(const char* file) {
		read(file);
	}

	//п.3 Разложить на компоненты
	void RGBcomponent(const char* file) {
		read(file);
		write("compR.bmp", 'r');
		write("compG.bmp", 'g');
		write("compB.bmp", 'b');
	}

	//п.4 корреляция
	void corel(const char* file) {
		read(file);
		//п.4(а)
		cout << "p. 4(a): ";
		cout << "\ncorrelation Red-Green is : " << getCorrelation('r', 'g') << "\n";
		cout << "correlation Green-Blue is : " << getCorrelation('g', 'b') << "\n";
		cout << "correlation Red-Blue is : " << getCorrelation('r', 'b') << "\n";
		cout << "BfOffBits: " << file_header.bfOffBits << std::endl;
		//п.4(б)
		vector<vector<double>> outCorrFirst;
		vector<vector<double>> outCorrSecond;
		vector<vector<double>> outCorrThird;
		for (int i = -10; i <= 10; i += 5) {
			vector<double> tmpFirst;
			vector<double> tmpSecond;
			vector<double> tmpThird;
			for (int j = -info_header.biWidth / 8; j < info_header.biWidth / 8; j++) {
				tmpFirst.push_back(getCorrelation('r', 'r', i, j));
				tmpSecond.push_back(getCorrelation('g', 'g', i, j));
				tmpThird.push_back(getCorrelation('b', 'b', i, j));
			}
			outCorrFirst.push_back(tmpFirst);
			outCorrSecond.push_back(tmpSecond);
			outCorrThird.push_back(tmpThird);
		}
		//red
		ofstream out;
		for (int i = 0; i < 5; i++) {
			string str = "RGr" + to_string(i) + ".dat";
			out.open(str);
			int idx = outCorrFirst[i].size() / 2;
			for (int j = 0; j < outCorrFirst[i].size(); j++) {
				if (j != outCorrFirst[i].size() - 1)
					out << outCorrFirst[i][j] << " " << j + 1 - idx << "\n";
				else
					out << outCorrFirst[i][j] << " " << j + 1 - idx;
			}
			out.close();
		}
		//green
		for (int i = 0; i < 5; i++) {
			string str = "GGr" + to_string(i) + ".dat";
			out.open(str);
			int idx = outCorrSecond[i].size() / 2;
			for (int j = 0; j < outCorrSecond[i].size(); j++) {
				if (j != outCorrSecond[i].size() - 1)
					out << outCorrSecond[i][j] << " " << j + 1 - idx << "\n";
				else
					out << outCorrSecond[i][j] << " " << j + 1 - idx;
			}
			out.close();
		}
		//blue
		for (int i = 0; i < 5; i++) {
			string str = "BGr" + to_string(i) + ".dat";
			out.open(str);
			int idx = outCorrThird[i].size() / 2;
			for (int j = 0; j < outCorrThird[i].size(); j++) {
				if (j != outCorrThird[i].size() - 1)
					out << outCorrThird[i][j] << " " << j + 1 - idx << "\n";
				else
					out << outCorrThird[i][j] << " " << j + 1 - idx;
			}
			out.close();
		}

	}

	//п.5 Перевод в формат YCbCr
	void RGBtoYCbCr(const char* file) {
		read(file);
		YCbCr strct;
		for (int i = 0; i < vec.size(); i++) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size(); j++) {
				strct.Y = 0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue;
				strct.Cb = 0.5643 * (double)(vec[i][j].rgbtBlue - strct.Y) + 128;
				strct.Cr = 0.7132 * (double)(vec[i][j].rgbtRed - strct.Y) + 128;
				tmp.push_back(strct);
			}
			vecYCbCr.push_back(tmp);
		}
		cout << "\np. 5 ";
		cout << "\ncorrelation YCb : " << getCorrelationYCbCr('Y', 'B') << "\n";
		cout << "correlation CbCr : " << getCorrelationYCbCr('B', 'R') << "\n";
		cout << "correlation YCr  : " << getCorrelationYCbCr('Y', 'R') << "\n";
		vecYCbCr.clear();
	}

	//п.6 Градации серого
	void GreyPictures(const char* file) {
		read(file);

		YCbCr strct;
		for (int i = 0; i < vec.size(); i++) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size(); j++) {
				strct.Y = 0.299 * vec[i][j].rgbtRed + 0.587 * vec[i][j].rgbtGreen + 0.114 * vec[i][j].rgbtBlue;
				strct.Cb = 0.5643 * (vec[i][j].rgbtBlue - strct.Y) + 128;
				strct.Cr = 0.7132 * (vec[i][j].rgbtRed - strct.Y) + 128;
				tmp.push_back(strct);
			}
			vecYCbCr.push_back(tmp);
		}

		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < vec[i].size(); j++) {
				vec[i][j].rgbtRed = vecYCbCr[i][j].Y;
				vec[i][j].rgbtGreen = vecYCbCr[i][j].Y;
				vec[i][j].rgbtBlue = vecYCbCr[i][j].Y;
			}
		}
		write("compY.bmp");

		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < vec[i].size(); j++) {
				vec[i][j].rgbtRed = vecYCbCr[i][j].Cb;
				vec[i][j].rgbtGreen = vecYCbCr[i][j].Cb;
				vec[i][j].rgbtBlue = vecYCbCr[i][j].Cb;
			}
		}
		write("compCb.bmp");

		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < vec[i].size(); j++) {
				vec[i][j].rgbtRed = vecYCbCr[i][j].Cr;
				vec[i][j].rgbtGreen = vecYCbCr[i][j].Cr;
				vec[i][j].rgbtBlue = vecYCbCr[i][j].Cr;
			}
		}
		write("compCr.bmp");
	}

	//p.7
	void PSNR(const char* file) {
		read(file);

		YCbCr strct;
		for (int i = 0; i < vec.size(); i++) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size(); j++) {
				strct.Y = 0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue;
				strct.Cb = 0.5643 * ((double)vec[i][j].rgbtBlue - strct.Y) + 128;
				strct.Cr = 0.7132 * ((double)vec[i][j].rgbtRed - strct.Y) + 128;
				tmp.push_back(strct);
			}
			vecYCbCr.push_back(tmp);
		}

		vvRGB translate;
		RGBTRIPLE pixel;
		int count = 0;
		for (int i = 0; i < vec.size(); i++) {
			vector<RGBTRIPLE> tmp;
			for (int j = 0; j < vec[i].size(); j++) {
				int r = vecYCbCr[i][j].Y - 0.714 * (vecYCbCr[i][j].Cr - 128) - 0.334 * (vecYCbCr[i][j].Cb - 128);
				int g = vecYCbCr[i][j].Y + 1.402 * (vecYCbCr[i][j].Cr - 128);
				int b = vecYCbCr[i][j].Y + 1.772 * (vecYCbCr[i][j].Cb - 128);
				if (r < 0) {
					r = 0;
				}
				if (r > 255)
					r = 255;
				if (g < 0)
					g = 0;
				if (g > 255)
					g = 255;
				if (b < 0)
					b = 0;
				if (b > 255)
					b = 255;

				pixel.rgbtGreen = r;
				pixel.rgbtRed = g;
				pixel.rgbtBlue = b;
				tmp.push_back(pixel);
			}

			translate.push_back(tmp);
		}


		double H = (double)info_header.biHeight;
		double W = (double)info_header.biWidth;
		double differenceR = 0;
		double differenceG = 0;
		double differenceB = 0;
		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < vec[i].size(); j++) {
				differenceR += pow(vec[i][j].rgbtRed - translate[i][j].rgbtRed, 2);
				differenceG += pow(vec[i][j].rgbtGreen - translate[i][j].rgbtGreen, 2);
				differenceB += pow(vec[i][j].rgbtBlue - translate[i][j].rgbtBlue, 2);
			}
		}
		double resR = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceR);
		double resG = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceG);
		double resB = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceB);
		cout << "PSNR for red is : " << resR << "\n";
		cout << "PSNR for green is : " << resG << "\n";
		cout << "PSNR for blue is : " << resB << "\n\n";


		for (int i = 0; i < vec.size(); i++)
			for (int j = 0; j < vec[i].size(); j++)
				vec[i][j] = translate[i][j];
		write("recover.bmp");
	}

	// p.8-10
	void part(const char* file, int step = 2) {
		//п.8
		read(file);
		vector<vector<YCbCr>> CbFirst;
		YCbCr strct;
		////////Cb Cr а/////
		for (int i = 0; i < vec.size(); i += step) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size(); j += step) {
				strct.Y = 0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue;
				strct.Cb = 0.5643 * (double)(vec[i][j].rgbtBlue - strct.Y) + 128;
				strct.Cr = 0.7132 * (double)(vec[i][j].rgbtRed - strct.Y) + 128;
				tmp.push_back(strct);
			}
			CbFirst.push_back(tmp);
		}
		////////Cb Cr b/////
		vector<vector<YCbCr>> CbSecond;
		for (int i = 0; i < vec.size(); i += step) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size(); j += step) {
				if (step == 2) {
					strct.Cb = (double)((0.5643 * (double)((double)vec[i][j].rgbtBlue - (0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue)) + 128) +
						(0.5643 * (double)((double)vec[i][j + 1].rgbtBlue - (0.299 * (double)vec[i][j + 1].rgbtRed + 0.587 * (double)vec[i][j + 1].rgbtGreen + 0.114 * (double)vec[i][j + 1].rgbtBlue)) + 128) +
						(0.5643 * (double)((double)vec[i + 1][j].rgbtBlue - (0.299 * (double)vec[i + 1][j].rgbtRed + 0.587 * (double)vec[i + 1][j].rgbtGreen + 0.114 * (double)vec[i + 1][j].rgbtBlue)) + 128) +
						(0.5643 * (double)((double)vec[i + 1][j + 1].rgbtBlue - (0.299 * (double)vec[i + 1][j + 1].rgbtRed + 0.587 * (double)vec[i + 1][j + 1].rgbtGreen + 0.114 * (double)vec[i + 1][j + 1].rgbtBlue)) + 128)) / 4;
					strct.Cr = (double)((0.7132 * (double)((double)vec[i][j].rgbtRed - (0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue)) + 128) +
						(0.7132 * (double)((double)vec[i][j + 1].rgbtRed - (0.299 * (double)vec[i][j + 1].rgbtRed + 0.587 * (double)vec[i][j + 1].rgbtGreen + 0.114 * (double)vec[i][j + 1].rgbtBlue)) + 128) +
						(0.7132 * (double)((double)vec[i + 1][j].rgbtRed - (0.299 * (double)vec[i + 1][j].rgbtRed + 0.587 * (double)vec[i + 1][j].rgbtGreen + 0.114 * (double)vec[i + 1][j].rgbtBlue)) + 128) +
						(0.7132 * (double)((double)vec[i + 1][j + 1].rgbtRed - (0.299 * (double)vec[i + 1][j + 1].rgbtRed + 0.587 * (double)vec[i + 1][j + 1].rgbtGreen + 0.114 * (double)vec[i + 1][j + 1].rgbtBlue)) + 128)) / 4;
				}
				if (step == 4) {
					strct.Cb = 0;
					for (int k = 0; k < 4; k++)
						for (int g = 0; g < 4; g++)
							strct.Cb += (0.5643 * (double)(vec[i + k][j + g].rgbtBlue - (0.299 * (double)vec[i + k][j + g].rgbtRed + 0.587 * (double)vec[i + k][j + g].rgbtGreen + 0.114 * (double)vec[i + k][j + g].rgbtBlue)) + 128);
					strct.Cb /= 16;

					strct.Cr = 0;
					for (int k = 0; k < 4; k++)
						for (int g = 0; g < 4; g++)
							strct.Cr += (0.7132 * (double)(vec[i + k][j + g].rgbtRed - (0.299 * (double)vec[i + k][j + g].rgbtRed + 0.587 * (double)vec[i + k][j + g].rgbtGreen + 0.114 * (double)vec[i + k][j + g].rgbtBlue)) + 128);
					strct.Cr /= 16;

				}
				tmp.push_back(strct);
			}
			CbSecond.push_back(tmp);
		}

		//п.9
		vector<vector<YCbCr>> recoveryA;
		for (int i = 0; i < vec.size() / step; i++) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size() / step; j++) {
				if (step == 4) {
					tmp.push_back(CbFirst[i][j]);
					tmp.push_back(CbFirst[i][j]);
				}
				tmp.push_back(CbFirst[i][j]);
				tmp.push_back(CbFirst[i][j]);
			}
			if (step == 4) {
				recoveryA.push_back(tmp);
				recoveryA.push_back(tmp);
			}
			recoveryA.push_back(tmp);
			recoveryA.push_back(tmp);
		}vector<vector<YCbCr>> recoveryB;
		for (int i = 0; i < vec.size() / step; i++) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size() / step; j++) {
				if (step == 4) {
					tmp.push_back(CbSecond[i][j]);
					tmp.push_back(CbSecond[i][j]);
				}
				tmp.push_back(CbSecond[i][j]);
				tmp.push_back(CbSecond[i][j]);
			}
			if (step == 4) {
				recoveryB.push_back(tmp);
				recoveryB.push_back(tmp);
			}
			recoveryB.push_back(tmp);
			recoveryB.push_back(tmp);
		}

		vector<vector<YCbCr>> CbCrAB;

		for (int i = 0; i < vec.size(); i++) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size(); j++) {
				recoveryA[i][j].Y = 0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue;
				recoveryB[i][j].Y = 0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue;
				strct.Y = 0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue;
				strct.Cb = 0.5643 * (double)(vec[i][j].rgbtBlue - strct.Y) + 128;
				strct.Cr = 0.7132 * (double)(vec[i][j].rgbtRed - strct.Y) + 128;
				tmp.push_back(strct);
			}
			CbCrAB.push_back(tmp);
		}

		vector<vector<RGBTRIPLE>> translateA;
		vector<vector<RGBTRIPLE>> translateB;
		RGBTRIPLE pixelA;
		RGBTRIPLE pixelB;

		for (int i = 0; i < vec.size(); i++) {
			vector<RGBTRIPLE> tmpA;
			vector<RGBTRIPLE> tmpB;
			int r = 0, g = 0, b = 0;
			for (int j = 0; j < vec[i].size(); j++) {
				g = recoveryA[i][j].Y - 0.714 * (recoveryA[i][j].Cr - 128) - 0.334 * (recoveryA[i][j].Cb - 128);
				r = recoveryA[i][j].Y + 1.402 * (recoveryA[i][j].Cr - 128);
				b = recoveryA[i][j].Y + 1.772 * (recoveryA[i][j].Cb - 128);
				if (r < 0)
					r = 0;
				if (r > 255)
					r = 255;
				if (g < 0)
					g = 0;
				if (g > 255)
					g = 255;
				if (b < 0)
					b = 0;
				if (b > 255)
					b = 255;

				pixelA.rgbtGreen = g;
				pixelA.rgbtRed = r;
				pixelA.rgbtBlue = b;
				tmpA.push_back(pixelA);

				g = recoveryB[i][j].Y - 0.714 * (recoveryB[i][j].Cr - 128) - 0.334 * (recoveryB[i][j].Cb - 128);
				r = recoveryB[i][j].Y + 1.402 * (recoveryB[i][j].Cr - 128);
				b = recoveryB[i][j].Y + 1.772 * (recoveryB[i][j].Cb - 128);

				if (r < 0)
					r = 0;
				if (r > 255)
					r = 255;
				if (g < 0)
					g = 0;
				if (g > 255)
					g = 255;
				if (b < 0)
					b = 0;
				if (b > 255)
					b = 255;

				pixelB.rgbtGreen = g;
				pixelB.rgbtRed = r;
				pixelB.rgbtBlue = b;
				tmpB.push_back(pixelB);
			}
			translateA.push_back(tmpA);
			translateB.push_back(tmpB);
		}
		for (int i = 0; i < vec.size(); i++)
			for (int j = 0; j < vec[i].size(); j++)
				vec[i][j] = translateA[i][j];
		write("Copy.bmp");

		for (int i = 0; i < vec.size(); i++)
			for (int j = 0; j < vec[i].size(); j++)
				vec[i][j] = translateB[i][j];
		write("Aver4.bmp");

		//п.10
		read(file);
		double H = (double)info_header.biHeight;
		double W = (double)info_header.biWidth;
		double differenceRA = 0;
		double differenceGA = 0;
		double differenceBA = 0;

		double differenceRB = 0;
		double differenceGB = 0;
		double differenceBB = 0;
		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < vec[i].size(); j++) {
				differenceRA += pow(vec[i][j].rgbtRed - translateA[i][j].rgbtRed, 2);
				differenceGA += pow(vec[i][j].rgbtGreen - translateA[i][j].rgbtGreen, 2);
				differenceBA += pow(vec[i][j].rgbtBlue - translateA[i][j].rgbtBlue, 2);
				differenceRB += pow(vec[i][j].rgbtRed - translateB[i][j].rgbtRed, 2);
				differenceGB += pow(vec[i][j].rgbtGreen - translateB[i][j].rgbtGreen, 2);
				differenceBB += pow(vec[i][j].rgbtBlue - translateB[i][j].rgbtBlue, 2);
			}
		}
		double resRA = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceRA);
		double resGA = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceGA);
		double resBA = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceBA);

		double resRB = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceRB);
		double resGB = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceGB);
		double resBB = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceBB);


		cout << "PSNR for red copy: " << resRA << "\n";
		cout << "PSNR for green copy: " << resGA << "\n";
		cout << "PSNR for blue copy: " << resBA << "\n\n";

		cout << "PSNR for red copy avg: " << resRB << "\n";
		cout << "PSNR for green copy avg: " << resGB << "\n";
		cout << "PSNR for blue copy avg: " << resBB << "\n\n";


		double differenceCbA = 0;
		double differenceCrA = 0;

		double differenceCbB = 0;
		double differenceCrB = 0;
		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < vec[i].size(); j++) {
				differenceCbA += pow(CbCrAB[i][j].Cb - recoveryA[i][j].Cb, 2);
				differenceCrA += pow(CbCrAB[i][j].Cr - recoveryA[i][j].Cr, 2);

				differenceCbB += pow(CbCrAB[i][j].Cb - recoveryB[i][j].Cb, 2);
				differenceCrB += pow(CbCrAB[i][j].Cr - recoveryB[i][j].Cr, 2);
			}
		}
		double resCbA = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceCbA);
		double resCrA = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceCrA);

		double resCbB = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceCbB);
		double resCrB = 10 * log10((W * H * pow((pow(2, 8) - 1), 2)) / differenceCrB);

		cout << "PSNR for Cb copy: " << resCbA << "\n";
		cout << "PSNR for Cr copy: " << resCrA << "\n\n";

		cout << "PSNR for Cb copy avg: " << resCbB << "\n";
		cout << "PSNR for Cr copy avg: " << resCrB << "\n\n";
	}

	//p11
	void part4(const char* file) {
		cout << "p.11 compress 4 \n";
		part(file, 4);
	}
	//p12-13

	void p12(const char* file) {
		read(file);
		double H = (double)info_header.biHeight;
		double W = (double)info_header.biWidth;
		vector<vector<YCbCr>> vecY;
		YCbCr strct;
		for (int i = 0; i < vec.size(); i++) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size(); j++) {
				strct.Y = 0.299 * vec[i][j].rgbtRed + 0.587 * vec[i][j].rgbtGreen + 0.114 * vec[i][j].rgbtBlue;
				strct.Cb = 0.5643 * (vec[i][j].rgbtBlue - strct.Y) + 128;
				strct.Cr = 0.7132 * (vec[i][j].rgbtRed - strct.Y) + 128;
				tmp.push_back(strct);
			}
			vecY.push_back(tmp);
		}

		int tmpR[256];
		int tmpG[256];
		int tmpB[256];
		int tmpCb[256];
		int tmpCr[256];
		int tmpY[256];
		for (int k = 0; k < 256; k++) {
			tmpR[k] = 0;
			tmpG[k] = 0;
			tmpB[k] = 0;
			tmpY[k] = 0;
			tmpCb[k] = 0;
			tmpCr[k] = 0;
		}
		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < vec[0].size(); j++) {
				tmpR[(int)vec[i][j].rgbtRed] += 1;
				tmpG[(int)vec[i][j].rgbtGreen] += 1;
				tmpB[(int)vec[i][j].rgbtBlue] += 1;
				tmpY[(int)vecY[i][j].Y] += 1;
				tmpCr[(int)vecY[i][j].Cr] += 1;
				tmpCb[(int)vecY[i][j].Cb] += 1;

			}
		}

		ofstream out;
		out.open("gistRGB.dat");
		for (int k = 0; k < 256; k++) {
			out << k << " " << tmpR[k] << " " << tmpG[k] << " " << tmpB[k] << " " << tmpCr[k] << " " << tmpCb[k] << " " << tmpY[k] << "\n";
		}
		out.close();
		double h = 0, p = 0, hg = 0, pg = 0, hb = 0, pb = 0, pCr = 0, pCb = 0, hCr = 0, hCb = 0, pY = 0, hY = 0;
		for (int i = 0; i < 256; i++) {
			p = (double)tmpR[i] / (W * H); //393216
			pg = (double)tmpG[i] / (W * H);
			pb = (double)tmpB[i] / (W * H);
			pY = (double)tmpY[i] / (W * H);
			pCr = (double)tmpCr[i] / (W * H);
			pCb = (double)tmpCb[i] / (W * H);
			if (p > 0)
				h += p * log2(p);
			if (pg > 0)
				hg += pg * log2(pg);
			if (pb > 0)
				hb += pb * log2(pb);
			if (pY > 0)
				hY += pY * log2(pY);
			if (pCr > 0)
				hCr += pCr * log2(pCr);
			if (pCb > 0)
				hCb += pCb * log2(pCb);
		}
		cout << "\n Task 13\n";
		cout << "H red = " << -h << "\n";
		cout << "H green = " << -hg << "\n";
		cout << "H blue = " << -hb << "\n";
		cout << "H Y = " << -hY << "\n";
		cout << "H Cb = " << -hCb << "\n";
		cout << "H Cr = " << -hCr << "\n";

	}

	void part14_15_16(const char* file) {
		read(file);
		vecYCbCr.clear();
		YCbCr strct;
		for (int i = 0; i < vec.size(); i++) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size(); j++) {
				strct.Y = 0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue;
				strct.Cb = 0.5643 * (double)(vec[i][j].rgbtBlue - strct.Y) + 128;
				strct.Cr = 0.7132 * (double)(vec[i][j].rgbtRed - strct.Y) + 128;
				tmp.push_back(strct);
			}
			vecYCbCr.push_back(tmp);
		}

		vvi Dr, Dg, Db;
		vvd Dy, Dcb, Dcr;
		for (int k = 1; k < 5; k++) {
			for (int i = 1; i < vec.size(); i++) {
				vector< int > tmpR, tmpG, tmpB;
				vector< double > tmpY, tmpCb, tmpCr;
				for (int j = 1; j < vec[i].size(); j++) {
					if (k == 1) {
						tmpR.push_back(vec[i][j].rgbtRed - vec[i][j - 1].rgbtRed);
						tmpG.push_back(vec[i][j].rgbtGreen - vec[i][j - 1].rgbtGreen);
						tmpB.push_back(vec[i][j].rgbtBlue - vec[i][j - 1].rgbtBlue);

						tmpY.push_back(vecYCbCr[i][j].Y - vecYCbCr[i][j - 1].Y);
						tmpCb.push_back(vecYCbCr[i][j].Cb - vecYCbCr[i][j - 1].Cb);
						tmpCr.push_back(vecYCbCr[i][j].Cr - vecYCbCr[i][j - 1].Cr);
					}
					if (k == 2) {
						tmpR.push_back(vec[i][j].rgbtRed - vec[i - 1][j].rgbtRed);
						tmpG.push_back(vec[i][j].rgbtGreen - vec[i - 1][j].rgbtGreen);
						tmpB.push_back(vec[i][j].rgbtBlue - vec[i - 1][j].rgbtBlue);

						tmpY.push_back(vecYCbCr[i][j].Y - vecYCbCr[i - 1][j].Y);
						tmpCb.push_back(vecYCbCr[i][j].Cb - vecYCbCr[i - 1][j].Cb);
						tmpCr.push_back(vecYCbCr[i][j].Cr - vecYCbCr[i - 1][j].Cr);
					}
					if (k == 3) {
						tmpR.push_back(vec[i][j].rgbtRed - vec[i - 1][j - 1].rgbtRed);
						tmpG.push_back(vec[i][j].rgbtGreen - vec[i - 1][j - 1].rgbtGreen);
						tmpB.push_back(vec[i][j].rgbtBlue - vec[i - 1][j - 1].rgbtBlue);

						tmpY.push_back(vecYCbCr[i][j].Y - vecYCbCr[i - 1][j - 1].Y);
						tmpCb.push_back(vecYCbCr[i][j].Cb - vecYCbCr[i - 1][j - 1].Cb);
						tmpCr.push_back(vecYCbCr[i][j].Cr - vecYCbCr[i - 1][j - 1].Cr);
					}
					if (k == 4) {
						tmpR.push_back(vec[i][j].rgbtRed - ((vec[i][j - 1].rgbtRed + vec[i - 1][j].rgbtRed + vec[i - 1][j - 1].rgbtRed) / 3));
						tmpG.push_back(vec[i][j].rgbtGreen - ((vec[i][j - 1].rgbtGreen + vec[i - 1][j].rgbtGreen + vec[i - 1][j - 1].rgbtGreen) / 3));
						tmpB.push_back(vec[i][j].rgbtBlue - ((vec[i][j - 1].rgbtBlue + vec[i - 1][j].rgbtBlue + vec[i - 1][j - 1].rgbtBlue) / 3));

						tmpY.push_back(vecYCbCr[i][j].Y - ((vecYCbCr[i][j - 1].Y + vecYCbCr[i - 1][j].Y + vecYCbCr[i - 1][j - 1].Y) / 3));
						tmpCb.push_back(vecYCbCr[i][j].Cb - ((vecYCbCr[i][j - 1].Cb + vecYCbCr[i - 1][j].Cb + vecYCbCr[i - 1][j - 1].Cb) / 3));
						tmpCr.push_back(vecYCbCr[i][j].Cr - ((vecYCbCr[i][j - 1].Cr + vecYCbCr[i - 1][j].Cr + vecYCbCr[i - 1][j - 1].Cr) / 3));
					}
				}
				Dr.push_back(tmpR);
				Dg.push_back(tmpG);
				Db.push_back(tmpB);
				Dy.push_back(tmpY);
				Dcb.push_back(tmpCb);
				Dcr.push_back(tmpCr);
			}
			if (k == 1)
				writeGist("gistRule1.dat", Dr, Dg, Db, Dy, Dcb, Dcr, 1);
			if (k == 2)
				writeGist("gistRule2.dat", Dr, Dg, Db, Dy, Dcb, Dcr, 2);
			if (k == 3)
				writeGist("gistRule3.dat", Dr, Dg, Db, Dy, Dcb, Dcr, 3);
			if (k == 4)
				writeGist("gistRule4.dat", Dr, Dg, Db, Dy, Dcb, Dcr, 4);
			Dr.clear();
			Dg.clear();
			Db.clear();
			Dy.clear();
			Dcb.clear();
			Dcr.clear();
		}
	}

	void part17(const char* file) {
		read(file);
		vector<vector<byte>> Y1;
		vector<vector<byte>> Y2;
		vector<vector<byte>> Y3;
		vector<vector<byte>> Y4;
		vector<vector<byte>> Y5;
		vector<vector<byte>> Y6;
		vector<vector<byte>> Y7;
		vector<vector<byte>> Y8;
		YCbCr strct;
		for (int i = 0; i < vec.size(); i++) {
			vector<YCbCr> tmp;
			for (int j = 0; j < vec[i].size(); j++) {
				strct.Y = 0.299 * (double)vec[i][j].rgbtRed + 0.587 * (double)vec[i][j].rgbtGreen + 0.114 * (double)vec[i][j].rgbtBlue;
				strct.Cb = 0.5643 * (double)(vec[i][j].rgbtBlue - strct.Y) + 128;
				strct.Cr = 0.7132 * (double)(vec[i][j].rgbtRed - strct.Y) + 128;
				tmp.push_back(strct);
			}
			vecYCbCr.push_back(tmp);
		}

		for (int i = 0; i < vecYCbCr.size(); i++) {
			vector<byte> y1, y2, y3, y4, y5, y6, y7, y8;
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				y1.push_back((((byte)vecYCbCr[i][j].Y & 1 << 7) != 0) * 255);
				y2.push_back((((byte)vecYCbCr[i][j].Y & 1 << 6) != 0) * 255);
				y3.push_back((((byte)vecYCbCr[i][j].Y & 1 << 5) != 0) * 255);
				y4.push_back((((byte)vecYCbCr[i][j].Y & 1 << 4) != 0) * 255);
				y5.push_back((((byte)vecYCbCr[i][j].Y & 1 << 3) != 0) * 255);
				y6.push_back((((byte)vecYCbCr[i][j].Y & 1 << 2) != 0) * 255);
				y7.push_back((((byte)vecYCbCr[i][j].Y & 1 << 1) != 0) * 255);
				y8.push_back((((byte)vecYCbCr[i][j].Y & 1) != 0) * 255);

			}
			Y1.push_back(y1);
			Y2.push_back(y2);
			Y3.push_back(y3);
			Y4.push_back(y4);
			Y5.push_back(y5);
			Y6.push_back(y6);
			Y7.push_back(y7);
			Y8.push_back(y8);
		}

		for (int i = 0; i < vecYCbCr.size(); i++) {
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				vecYCbCr[i][j].Y = (double)Y4[i][j]; // тут менять чтобы отобразить любое из 8 изображений
			}
		}

		vec.clear();
		RGBTRIPLE pixel;
		for (int i = 0; i < vecYCbCr.size(); i++) {
			vector<RGBTRIPLE> tmp;
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				pixel.rgbtGreen = Y1[i][j];
				pixel.rgbtRed = Y1[i][j];
				pixel.rgbtBlue = Y1[i][j];
				tmp.push_back(pixel);
			}
			vec.push_back(tmp);
		}
		write("Task17pt1.bmp");
		vec.clear();
		for (int i = 0; i < vecYCbCr.size(); i++) {
			vector<RGBTRIPLE> tmp;
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				pixel.rgbtGreen = Y2[i][j];
				pixel.rgbtRed = Y2[i][j];
				pixel.rgbtBlue = Y2[i][j];
				tmp.push_back(pixel);
			}
			vec.push_back(tmp);
		}
		write("17pynkt2.bmp");

		vec.clear();

		for (int i = 0; i < vecYCbCr.size(); i++) {
			vector<RGBTRIPLE> tmp;
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				pixel.rgbtGreen = Y3[i][j];
				pixel.rgbtRed = Y3[i][j];
				pixel.rgbtBlue = Y3[i][j];
				tmp.push_back(pixel);
			}
			vec.push_back(tmp);
		}
		write("17pynkt3.bmp");
		vec.clear();
		for (int i = 0; i < vecYCbCr.size(); i++) {
			vector<RGBTRIPLE> tmp;
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				pixel.rgbtGreen = Y4[i][j];
				pixel.rgbtRed = Y4[i][j];
				pixel.rgbtBlue = Y4[i][j];
				tmp.push_back(pixel);
			}
			vec.push_back(tmp);
		}
		write("17pynkt4.bmp");
		vec.clear();
		for (int i = 0; i < vecYCbCr.size(); i++) {
			vector<RGBTRIPLE> tmp;
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				pixel.rgbtGreen = Y5[i][j];
				pixel.rgbtRed = Y5[i][j];
				pixel.rgbtBlue = Y5[i][j];
				tmp.push_back(pixel);
			}
			vec.push_back(tmp);
		}
		write("17pynkt5.bmp");
		vec.clear();
		for (int i = 0; i < vecYCbCr.size(); i++) {
			vector<RGBTRIPLE> tmp;
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				pixel.rgbtGreen = Y6[i][j];
				pixel.rgbtRed = Y6[i][j];
				pixel.rgbtBlue = Y6[i][j];
				tmp.push_back(pixel);
			}
			vec.push_back(tmp);
		}
		write("17pynkt6.bmp");
		vec.clear();

		for (int i = 0; i < vecYCbCr.size(); i++) {
			vector<RGBTRIPLE> tmp;
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				pixel.rgbtGreen = Y7[i][j];
				pixel.rgbtRed = Y7[i][j];
				pixel.rgbtBlue = Y7[i][j];
				tmp.push_back(pixel);
			}
			vec.push_back(tmp);
		}
		write("17pynkt7.bmp");
		vec.clear();

		for (int i = 0; i < vecYCbCr.size(); i++) {
			vector<RGBTRIPLE> tmp;
			for (int j = 0; j < vecYCbCr[i].size(); j++) {
				pixel.rgbtGreen = Y8[i][j];
				pixel.rgbtRed = Y8[i][j];
				pixel.rgbtBlue = Y8[i][j];
				tmp.push_back(pixel);
			}
			vec.push_back(tmp);
		}
		write("17pynkt8.bmp");
	}
};



int main() {
	Picture* a = new Picture();
	setlocale(LC_ALL, "Russian");
	a->readPict(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)"); //п.2
	a->RGBcomponent(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)"); //п.3
//	a->corel(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)"); //п.4
//	a->RGBtoYCbCr(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)"); //п.5
//	a->GreyPictures(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)"); //п.6
	cout << "Task 7" << endl;
	a->PSNR(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)"); // п.7
	cout << "Task 8-10" << endl;
	a->part(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)", 2); // п.8-10 децимация для 2
	cout << "Task 11" << endl;
	 a->part4(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)"); //п.11   для 4
	 cout << "Tasks 12,13" << endl;
	 a->p12(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)"); //п.12-13 гистограмма и энтропия
	 cout << "Tasks 14,15,16" << endl;
	 a->part14_15_16(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)");
//	 a->part17(R"(D:\CLion_projects\crypto\imageAnalysis1\data\srcImage.bmp)");
	return 0;
}