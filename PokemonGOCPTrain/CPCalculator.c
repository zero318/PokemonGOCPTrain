#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MinPokemon 1
#define MaxPokemon 964
#define MinLevel 1
#define MaxLevel 89
#define MinIV 0
#define MaxIV 15
#define IVCount 16
#define MinCP 1
#define MaxCP 10403
#define MinCPM 1
#define MaxCPM 45
#define BinaryMode 0
#define CSVMode 1
#define ODSMode 2
#define XLSXMode 3
#define CalcMode XLSXMode

#define PrintCrap

#define DoWrap(OtherMacro) \
do {\
	OtherMacro\
} while (0)

//This macro is literally just a toggleable printf.
//The syntax is the same as the regular printf, but
//it can be toggled on/off by defining PrintCrap
#ifdef PrintCrap
#define CrappyPrintf(...) DoWrap((void)printf(__VA_ARGS__);)
#else
#define CrappyPrintf(...) DoWrap(/*yeet*/)
#endif

//These macros are essentially just malloc/calloc with
//inlined error checking. Do NOT use them in an if/else
//structure without braces, otherwise the compiler gets AIDS
#define SaferMalloc(Type, Pointer, Size) \
(Type*)malloc(Size);\
if (!Pointer) {\
	printf("malloc of size %zu failed!\n", (size_t)Size);\
	exit(1);\
}
#define SaferCalloc(Type, Pointer, Count, Size) \
(Type*)calloc(Count, Size);\
if (!Pointer) {\
	printf("calloc of count %llu and size %zu failed!\n", (long long)Count, (size_t)Size);\
	exit(1);\
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <Windows.h>

#pragma pack(push, 1)
typedef struct PokemonStatsStruct {
	uint32_t BaseHP : 10, BaseDefense : 10, BaseAttack : 10, : 0;
	uint16_t Form : 3, Dex : 10, : 3;
} PokemonStatsStruct;

typedef struct CPComboStruct {
	uint32_t HPIV : 4, DefenseIV : 4, AttackIV : 4, Level : 7, Form : 3, Dex : 10;
} CPComboStruct;

typedef struct {
	uint_fast32_t Height;
	CPComboStruct* Data;
} CPColumnMetadata;
#pragma pack(pop)

void main() {
	//Set the process priority as high as possible since maybe it'll run faster
	HANDLE CalculatorProcess = GetCurrentProcess();
	SetPriorityClass(CalculatorProcess, REALTIME_PRIORITY_CLASS);
	CrappyPrintf("Pokemon GO CP Combination Calculator\nLoading CPMs\n%u\n", MaxLevel - 1);
	//Load CPM values from the data file and calculate the double values used later
	FILE *PokemonStatsFile = fopen(".\\PokemonStats.bin", "rb");
	double CPM[MaxLevel];
	{
		uint_fast8_t CPMIndex = 0;
		float CPMFloats[MaxCPM];
		(void)fread(&CPMFloats, MaxCPM, sizeof(float), PokemonStatsFile);
		for (uint_fast8_t Level = 0; Level < MaxLevel; ++Level) {
			CrappyPrintf("%u\r", Level);
			if (Level & 1) {
#pragma warning(suppress:26451)
				CPM[Level] = sqrt(CPMFloats[CPMIndex] * CPMFloats[CPMIndex] - CPMFloats[CPMIndex] * CPMFloats[CPMIndex] / 2 + CPMFloats[CPMIndex + 1] * CPMFloats[CPMIndex + 1] / 2);
				++CPMIndex;
			}
			else {
				CPM[Level] = (double)CPMFloats[CPMIndex];
			}
			//Square all the CPM values to make later calculations easier
			CPM[Level] *= CPM[Level];
		}
	}
	//Load the pokemon stats from a file
	PokemonStatsStruct* PokemonStats = SaferMalloc(PokemonStatsStruct, PokemonStats, MaxPokemon * sizeof(PokemonStatsStruct));
	(void)fread(PokemonStats, MaxPokemon, 6, PokemonStatsFile);
	(void)fclose(PokemonStatsFile);
	//This line needs to be calloc instead of malloc to initialize Height to 0 without looping the array
	CPColumnMetadata* CPColumn = SaferCalloc(CPColumnMetadata, CPColumn, MaxCP, sizeof(CPColumnMetadata));
	//PokemonOutputUnion* CachedCPs = SaferMalloc(PokemonOutputUnion, CachedCPs, MaxPokemon * MaxLevel * IVCount * IVCount * IVCount * sizeof(PokemonOutputUnion));
	uint16_t* CachedCPs = SaferMalloc(uint16_t, CachedCPs, MaxPokemon * MaxLevel * IVCount * IVCount * IVCount * sizeof(uint16_t));
	uint_fast32_t CPIndex = 0;
	//This struct is used for holding many temporary values
	CPComboStruct Combo = { .AttackIV = 0, .DefenseIV = 0, .HPIV = 0 };
	CrappyPrintf("\nPokemon Count Pass 1:\n%u\n", MaxPokemon - 1);
	for (uint_fast16_t Pokemon = 0; Pokemon < MaxPokemon; ++Pokemon) {
		CrappyPrintf("%u\r", Pokemon);
		for (Combo.Level = MinLevel - 1; Combo.Level < MaxLevel; ++Combo.Level) {
			//I couldn't get this to work in a for loop after
			//several hours or trying. Intentional overflow FTW I guess
			do {
				do {
					do {
#pragma warning(suppress:6011)
#pragma warning(suppress:6385)
#pragma warning(suppress:26451)
						//This counts how much data is going to be in each column
						//of the final output sheet and stores the CP values to a list.
						//This list could also be used for generating sharedstrings
						CachedCPs[CPIndex] = (uint16_t)((((PokemonStats[Pokemon].BaseAttack + Combo.AttackIV) * sqrt(PokemonStats[Pokemon].BaseDefense + Combo.DefenseIV) * sqrt(PokemonStats[Pokemon].BaseHP + Combo.HPIV) * CPM[Combo.Level]) / 10) - 1);
						++CPColumn[CachedCPs[CPIndex]].Height;
						++CPIndex;
					} while (Combo.HPIV++ < MaxIV);
				} while (Combo.DefenseIV++ < MaxIV);
			} while (Combo.AttackIV++ < MaxIV);
		}
	}
	uint_fast16_t CP;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		if (CPColumn[CP].Height) {
			CPColumn[CP].Data = SaferMalloc(CPComboStruct, CPColumn[CP].Data, CPColumn[CP].Height * sizeof(CPComboStruct));
			CPColumn[CP].Height = 0;
		}
	}
	CrappyPrintf("\nPokemon Count Pass 2:\n%u\n", MaxPokemon - 1);
	CPIndex = 0;
	for (uint_fast16_t Pokemon = 0; Pokemon < MaxPokemon; ++Pokemon) {
		CrappyPrintf("%u\r", Pokemon);
		Combo.Dex = PokemonStats[Pokemon].Dex;
		Combo.Form = PokemonStats[Pokemon].Form;
		for (Combo.Level = MinLevel; Combo.Level <= MaxLevel; ++Combo.Level) {
			do {
				do {
					do {
						//This (ab)uses the fact that the combinations are looped in the same order
						//as the previous loop, so the CP values can be reused instead of recalculated
						CPColumn[CachedCPs[CPIndex]].Data[CPColumn[CachedCPs[CPIndex]].Height] = Combo;
						++CPColumn[CachedCPs[CPIndex]].Height;
						//CPColumn[CachedCPs[CPIndex].CPValue].Data[CPColumn[CachedCPs[CPIndex].CPValue].Height].CPStringIndex = CPIndex;
						//CachedCPs[CPIndex].Struct = Combo;
						++CPIndex;
					} while (Combo.HPIV++ < MaxIV);
				} while (Combo.DefenseIV++ < MaxIV);
			} while (Combo.AttackIV++ < MaxIV);
		}
	}
	free(PokemonStats);
	free(CachedCPs);
	//CrappyPrintf("\nCP Count:\n%u\n", CPIndex - 1);
#if CalcMode == BinaryMode
	FILE* PokemonOutputFile = fopen(".\\PokemonCPOutput.bin", "w+b");
	CPComboStruct ColumnSpacer = { .HPIV = 0, .DefenseIV = 0, .AttackIV = 0, .Level = 0, .Form = 0, .Dex = 0 };
	//PokemonOutputUnion ColumnSpacer = { .Struct.HPIV = 0, .Struct.DefenseIV = 0, .Struct.AttackIV = 0, .Struct.Level = 0, .Struct.Form = 0, .Struct.Dex = 0 };
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		CrappyPrintf("%u\r", CP);
		if (CPColumn[CP].Height) {
			for (uint_fast32_t PokemonOutput = 0; PokemonOutput < CPColumn[CP].Height; ++PokemonOutput) {
#pragma warning(suppress:6001)
				(void)fwrite(CPColumn[CP].Data[PokemonOutput], 1, 4, PokemonOutputFile);
				//(void)fwrite(CPColumn[CP].ColumnData[PokemonOutput].OutChars, 1, 4, PokemonOutputFile);
			}
			free(CPColumn[CP].Data);
			(void)fwrite(ColumnSpacer, 1, 4, PokemonOutputFile);
			//free(CPColumn[CP].ColumnData);
			//(void)fwrite(ColumnSpacer.OutChars, 1, 4, PokemonOutputFile);
		}
	}
	free(CPColumn);
	(void)fclose(PokemonOutputFile);
#elif CalcMode == CSVMode
	FILE* SharedStrings = fopen("PokemonCPStrings.csv", "w+");
	for (CP = 0; CP < CPIndex; ++CP) {
		//CrappyPrintf("%u\r", CP);
		(void)fprintf(SharedStrings, "%3u%o%2u%X%X%X\n", CachedCPs[CP].Struct.Dex, CachedCPs[CP].Struct.Form, CachedCPs[CP].Struct.Level, CachedCPs[CP].Struct.AttackIV, CachedCPs[CP].Struct.DefenseIV, CachedCPs[CP].Struct.HPIV);
	}
	free(CachedCPs);
	(void)fclose(SharedStrings);
	uint_fast32_t MaxRow = 0;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		if (CPColumn[CP].Height) {
			if (CPColumn[CP].Height > MaxRow) {
				MaxRow = CPColumn[CP].Height;
			}
		}
	}
	CrappyPrintf("\nMaxCount: %u\nCount:\n", MaxRow);
	FILE* PokemonOutputFile = fopen("PokemonCPOutput.csv", "w+");
	for (uint_fast32_t Row = 0; Row < MaxRow; ++Row) {
		CrappyPrintf("%u\r", Row);
		if (Row < CPColumn[0].Height) {
			(void)fprintf(PokemonOutputFile, "%u", CPColumn[0].ColumnData[Row].CPStringIndex);
			//(void)fprintf(PokemonOutputFile, "%3u%o%2u%X%X%X", CPColumn[0].ColumnData[Row].Struct.Dex, CPColumn[0].ColumnData[Row].Struct.Form, CPColumn[0].ColumnData[Row].Struct.Level, CPColumn[0].ColumnData[Row].Struct.AttackIV, CPColumn[0].ColumnData[Row].Struct.DefenseIV, CPColumn[0].ColumnData[Row].Struct.HPIV);
		}
		for (CP = MinCP; CP < MaxCP; ++CP) {
			if (Row < CPColumn[CP].Height) {
				(void)fprintf(PokemonOutputFile, ",%u", CPColumn[CP].ColumnData[Row].CPStringIndex);
				//(void)fprintf(PokemonOutputFile, ",%3u%o%2u%X%X%X", CPColumn[CP].ColumnData[Row].Struct.Dex, CPColumn[CP].ColumnData[Row].Struct.Form, CPColumn[CP].ColumnData[Row].Struct.Level, CPColumn[CP].ColumnData[Row].Struct.AttackIV, CPColumn[CP].ColumnData[Row].Struct.DefenseIV, CPColumn[CP].ColumnData[Row].Struct.HPIV);
			}
			else {
				(void)fprintf(PokemonOutputFile, ",");
			}
		}
		(void)fprintf(PokemonOutputFile, "\n");
	}
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		free(CPColumn[CP].ColumnData);
	}
	free(CPColumn);
	(void)fclose(PokemonOutputFile);
#elif CalcMode == XLSXMode
	/*FILE* SharedStrings = fopen("F:\\My Programming Stuff Expansion\\ExcelFileTest\\xl\\sharedStrings.xml", "w+");
	(void)fprintf(SharedStrings, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" count=\"%u\" uniqueCount=\"%u\">", CPIndex, CPIndex);
	for (CP = 0; CP < CPIndex; ++CP) {
		CrappyPrintf("%u\r", CP)
		(void)fprintf(SharedStrings, "<si><t>%3u%o%2u%X%X%X</t></si>", CachedCPs[CP].Struct.Dex, CachedCPs[CP].Struct.Form, CachedCPs[CP].Struct.Level, CachedCPs[CP].Struct.AttackIV, CachedCPs[CP].Struct.DefenseIV, CachedCPs[CP].Struct.HPIV);
	}
	free(CachedCPs);
	(void)fprintf(SharedStrings, "</sst>");
	(void)fclose(SharedStrings);*/
	uint_fast32_t MaxRow = 0;
	//Calculate the maximum row the data can go down to
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		if (CPColumn[CP].Height) {
			if (CPColumn[CP].Height > MaxRow) {
				MaxRow = CPColumn[CP].Height;
			}
		}
	}
	//Calculate the Left/Right bounds of the data in each row
	uint16_t* CPLeftBound = SaferMalloc(uint16_t, CPLeftBound, MaxRow * sizeof(uint16_t));
	uint16_t* CPRightBound = SaferMalloc(uint16_t, CPRightBound, MaxRow * sizeof(uint16_t));
	uint16_t MinCPBound = MinCP - 1;
	uint16_t MaxCPBound = MaxCP - 1;
	CrappyPrintf("\nRow Parsing 1:\n%u\n", MaxRow - 1);
	for (uint_fast32_t Row = 0; Row < MaxRow; ++Row) {
		CrappyPrintf("%u\r", Row);
		for (CP = MinCPBound; CP <= MaxCPBound; ++CP) {
			if (Row < CPColumn[CP].Height) {
				CPLeftBound[Row] = CP;
				if (CP > MinCPBound) {
					MinCPBound = CP;
				}
				break;
			}
		}
		for (CP = MaxCPBound; CP >= MinCPBound; --CP) {
			if (Row < CPColumn[CP].Height) {
				CPRightBound[Row] = CP;
				if (CP < MaxCPBound) {
					MaxCPBound = CP;
				}
				break;
			}
		}
	}
	//Load a file with a bunch of strings representing Excel's column headers
	//Because screw trying to actually calcuate those dang letters on the fly
	//All indices are multiplied by 4 to account for max string length
	char* EndMySuffering = SaferMalloc(char, EndMySuffering, MaxCP * 4);
	FILE * AAAAAAAAH = fopen("FML.bin", "rb");
	(void)fread(EndMySuffering, MaxCP * 4, sizeof(char), AAAAAAAAH);
	(void)fclose(AAAAAAAAH);
	CrappyPrintf("\nRow Parsing 2:\n%u\n", MaxRow - 1);
	//Start printing the actual XML
	FILE* PokemonOutputFile = fopen("F:\\My Programming Stuff Expansion\\ExcelFileTest\\xl\\worksheets\\sheet1.xml", "w+");
	(void)fprintf(PokemonOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" mc:Ignorable=\"x14ac\" xmlns:x14ac=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac\"><dimension ref=\"A1:%s%u\"/><sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"/></sheetViews><sheetFormatPr defaultRowHeight=\"15\" x14ac:dyDescent=\"0.25\"/><sheetData>", &EndMySuffering[MaxCP * 4 - 4], MaxRow);
	for (uint_fast32_t Row = 0; Row < MaxRow; ++Row) {
		CrappyPrintf("%u\r", Row);
		(void)fprintf(PokemonOutputFile, "<row r=\"%u\" spans=\"1:3\" x14ac:dyDescent=\"0.25\">", Row);
		//Use the bounding data to only loop where the data is
		for (CP = CPLeftBound[Row]; CP <= CPRightBound[Row]; ++CP) {
		//for (CP = MinCP; CP < MaxCP; ++CP) {
			if (Row < CPColumn[CP].Height) {
				(void)fprintf(PokemonOutputFile, "<c r=\"%s%u\"><v>%03u%u%02u%02u%02u%02u</v></c>", &EndMySuffering[CP * 4], Row + 1, CPColumn[CP].Data[Row].Dex, CPColumn[CP].Data[Row].Form, CPColumn[CP].Data[Row].Level, CPColumn[CP].Data[Row].AttackIV, CPColumn[CP].Data[Row].DefenseIV, CPColumn[CP].Data[Row].HPIV);
				//(void)fprintf(PokemonOutputFile, "<c r=\"%s%u1\" t=\"s\"><v>%u</v></c>", &EndMySuffering[CP * 4], Row, CPColumn[CP].ColumnData[Row].CPStringIndex);
			}
		}
		(void)fprintf(PokemonOutputFile, "</row>");
	}
	//Get rid of that cancerous list of column strings already
	free(EndMySuffering);
	free(CPLeftBound);
	free(CPRightBound);
	//Print the XML footer
	(void)fprintf(PokemonOutputFile, "</sheetData><pageMargins left=\"0.7\" right=\"0.7\" top=\"0.75\" bottom=\"0.75\" header=\"0.3\" footer=\"0.3\"/></worksheet>");
	(void)fclose(PokemonOutputFile);
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		free(CPColumn[CP].Data);
	}
	free(CPColumn);
#endif
	CrappyPrintf("\nDone");
}