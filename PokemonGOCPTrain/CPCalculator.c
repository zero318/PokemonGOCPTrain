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

#ifdef PrintCrap
#define CrappyPrintf(StringyThingy, ...) (void)printf(StringyThingy, __VA_ARGS__);
#else
#define CrappyPrintf(StringyThingy, ...) /*yeet*/
#endif

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
typedef struct PokemonInputStruct {
	uint32_t BaseHP : 10, BaseDefense : 10, BaseAttack : 10, : 0;
	uint16_t Form : 3, Dex : 10, : 3;
} PokemonInputStruct;

typedef union {
	struct PokemonInputStruct;
	uint8_t InChars[6];
} PokemonInputData;

typedef union {
	float single;
	uint8_t InChars[4];
} CPMFloat;

typedef struct PokemonOutputStruct {
	uint32_t HPIV : 4, DefenseIV : 4, AttackIV : 4, Level : 7, Form : 3, Dex : 10;
} PokemonOutputStruct;

typedef union PokemonOutputUnion {
	uint32_t CPStringIndex;
	uint32_t CPValue;
	struct PokemonOutputStruct Struct;
	uint8_t OutChars[4];
} PokemonOutputUnion;

typedef struct {
	uint_fast32_t Count;
	union PokemonOutputUnion* ColumnData;
} CPMatchesData;
#pragma pack(pop)

void main() {
	HANDLE CalculatorProcess = GetCurrentProcess();
	SetPriorityClass(CalculatorProcess, REALTIME_PRIORITY_CLASS);
	CrappyPrintf("Pokemon GO CP Combination Calculator\nLoading CPMs\n%u\n", MaxLevel - 1)
	FILE *PokemonStatsFile = fopen(".\\PokemonStats.bin", "rb");
	double CPMDoublesSquared[MaxLevel];
	{//Lol, I don't need CPMFloats after this
		uint_fast8_t CPM = 0;
		CPMFloat CPMFloats[MaxCPM];
		(void)fread(&CPMFloats->InChars, MaxCPM, sizeof(float), PokemonStatsFile);
		for (uint_fast8_t Level = 0; Level < MaxLevel; ++Level) {
			CrappyPrintf("%u\r", Level);
			if (Level & 1) {
#pragma warning(suppress:26451)	//Can VS just shut up about my cast operation? It's supposed to be like that.
				CPMDoublesSquared[Level] = sqrt(CPMFloats[CPM].single * CPMFloats[CPM].single - CPMFloats[CPM].single * CPMFloats[CPM].single / 2 + CPMFloats[CPM + 1].single * CPMFloats[CPM + 1].single / 2);
				++CPM;
			}
			else {
				CPMDoublesSquared[Level] = (double)CPMFloats[CPM].single;
			}
			CPMDoublesSquared[Level] *= CPMDoublesSquared[Level];
		}
	}
	PokemonInputData* PokemonData = SaferMalloc(PokemonInputData, PokemonData, MaxPokemon * sizeof(PokemonInputData))
	(void)fread(PokemonData, MaxPokemon, 6, PokemonStatsFile);
	(void)fclose(PokemonStatsFile);
	//CPMatchesData* CPMatches = SaferMalloc(CPMatchesData, CPMatches, MaxCP * sizeof(CPMatchesData))
	CPMatchesData* CPMatches = SaferCalloc(CPMatchesData, CPMatches, MaxCP, sizeof(CPMatchesData))
	PokemonOutputUnion* CPList = SaferMalloc(PokemonOutputUnion, CPList, MaxPokemon * MaxLevel * IVCount * IVCount * IVCount * sizeof(PokemonOutputUnion))
	//uint16_t* CPList = SaferMalloc(uint16_t, CPList, MaxPokemon * MaxLevel * IVCount * IVCount * IVCount * sizeof(uint16_t))
	uint_fast32_t CPCount = 0;
	PokemonOutputStruct TempOutputStruct;
	TempOutputStruct.AttackIV = MaxIV;
	TempOutputStruct.DefenseIV = MaxIV;
	TempOutputStruct.HPIV = MaxIV;
	CrappyPrintf("\nPokemon Count Pass 1:\n%u\n", MaxPokemon - 1)
	for (uint_fast16_t Pokemon = 0; Pokemon < MaxPokemon; ++Pokemon) {
		CrappyPrintf("%u\r", Pokemon);
		for (TempOutputStruct.Level = MinLevel - 1; TempOutputStruct.Level < MaxLevel; ++TempOutputStruct.Level) {
			do {//Intentional overflow FTW I guess
				++TempOutputStruct.AttackIV;
				do {
					++TempOutputStruct.DefenseIV;
					do {
						++TempOutputStruct.HPIV;
#pragma warning(suppress:6011)
#pragma warning(suppress:6385)
#pragma warning(suppress:26451)
						CPList[CPCount].CPValue = (uint32_t)((((PokemonData[Pokemon].BaseAttack + TempOutputStruct.AttackIV) * sqrt(PokemonData[Pokemon].BaseDefense + TempOutputStruct.DefenseIV) * sqrt(PokemonData[Pokemon].BaseHP + TempOutputStruct.HPIV) * CPMDoublesSquared[TempOutputStruct.Level]) / 10) - 1);
						++CPMatches[CPList[CPCount].CPValue].Count;
						++CPCount;
					} while (TempOutputStruct.HPIV < MaxIV);
				} while (TempOutputStruct.DefenseIV < MaxIV);
			} while (TempOutputStruct.AttackIV < MaxIV);
		}
	}
	uint_fast16_t CP;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		if (CPMatches[CP].Count) {
			CPMatches[CP].ColumnData = SaferMalloc(PokemonOutputUnion, CPMatches[CP].ColumnData, CPMatches[CP].Count * sizeof(PokemonOutputUnion))
			//CPMatches[CP].ColumnData = SaferCalloc(PokemonOutputUnion, CPMatches[CP].ColumnData, CPMatches[CP].Count, sizeof(PokemonOutputUnion))
			CPMatches[CP].Count = 0;
		}
	}
	CrappyPrintf("\nPokemon Count Pass 2:\n%u\n", MaxPokemon - 1)
	CPCount = 0;
	for (uint_fast16_t Pokemon = 0; Pokemon < MaxPokemon; ++Pokemon) {
		CrappyPrintf("%u\r", Pokemon)
		TempOutputStruct.Dex = PokemonData[Pokemon].Dex;
		TempOutputStruct.Form = PokemonData[Pokemon].Form;
		for (TempOutputStruct.Level = MinLevel; TempOutputStruct.Level <= MaxLevel; ++TempOutputStruct.Level) {
			do {
				++TempOutputStruct.AttackIV;
				do {
					++TempOutputStruct.DefenseIV;
					do {
						++TempOutputStruct.HPIV;
						CPMatches[CPList[CPCount].CPValue].ColumnData[CPMatches[CPList[CPCount].CPValue].Count].CPStringIndex = CPCount;
						//CPMatches[CPList[CPCount]].ColumnData[CPMatches[CPList[CPCount]].Count].Struct = TempOutputStruct;
						++CPMatches[CPList[CPCount].CPValue].Count;
						CPList[CPCount].Struct = TempOutputStruct;
						++CPCount;
					} while (TempOutputStruct.HPIV < MaxIV);
					//TempOutputStruct.HPIV -= MaxIV;
				} while (TempOutputStruct.DefenseIV < MaxIV);
				//TempOutputStruct.DefenseIV -= MaxIV;
			} while (TempOutputStruct.AttackIV < MaxIV);
			//TempOutputStruct.AttackIV -= MaxIV;
		}
	}
	free(PokemonData);
	//free(CPList);
	CrappyPrintf("\nCPCount:\n%u\n", CPCount - 1)
#if CalcMode == BinaryMode
	FILE* PokemonOutputFile = fopen(".\\PokemonCPOutput.bin", "w+b");
	PokemonOutputUnion ColumnSpacer = { .Struct.HPIV = 0, .Struct.DefenseIV = 0, .Struct.AttackIV = 0, .Struct.Level = 0, .Struct.Form = 0, .Struct.Dex = 0 };
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		CrappyPrintf("%u\r", CP)
		if (CPMatches[CP].Count) {
			for (uint_fast32_t PokemonOutput = 0; PokemonOutput < CPMatches[CP].Count; ++PokemonOutput) {
#pragma warning(suppress:6001)
				(void)fwrite(CPMatches[CP].ColumnData[PokemonOutput].OutChars, 1, 4, PokemonOutputFile);
			}
			free(CPMatches[CP].ColumnData);
			(void)fwrite(ColumnSpacer.OutChars, 1, 4, PokemonOutputFile);
		}
	}
	free(CPMatches);
	(void)fclose(PokemonOutputFile);
#elif CalcMode == CSVMode
	FILE* SharedStrings = fopen("PokemonCPStrings.csv", "w+");
	for (CP = 0; CP < CPCount; ++CP) {
		//CrappyPrintf("%u\r", CP)
		(void)fprintf(SharedStrings, "%3u%o%2u%X%X%X\n", CPList[CP].Struct.Dex, CPList[CP].Struct.Form, CPList[CP].Struct.Level, CPList[CP].Struct.AttackIV, CPList[CP].Struct.DefenseIV, CPList[CP].Struct.HPIV);
	}
	free(CPList);
	(void)fclose(SharedStrings);
	uint_fast32_t MaxCount = 0;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		if (CPMatches[CP].Count) {
			if (CPMatches[CP].Count > MaxCount) {
				MaxCount = CPMatches[CP].Count;
			}
		}
	}
	CrappyPrintf("\nMaxCount: %u\nCount:\n", MaxCount)
	FILE* PokemonOutputFile = fopen("PokemonCPOutput.csv", "w+");
	for (uint_fast32_t CountIndex = 0; CountIndex < MaxCount; ++CountIndex) {
		CrappyPrintf("%u\r", CountIndex)
		if (CountIndex < CPMatches[0].Count) {
			(void)fprintf(PokemonOutputFile, "%u", CPMatches[0].ColumnData[CountIndex].CPStringIndex);
			//(void)fprintf(PokemonOutputFile, "%3u%o%2u%X%X%X", CPMatches[0].ColumnData[CountIndex].Struct.Dex, CPMatches[0].ColumnData[CountIndex].Struct.Form, CPMatches[0].ColumnData[CountIndex].Struct.Level, CPMatches[0].ColumnData[CountIndex].Struct.AttackIV, CPMatches[0].ColumnData[CountIndex].Struct.DefenseIV, CPMatches[0].ColumnData[CountIndex].Struct.HPIV);
		}
		for (CP = MinCP; CP < MaxCP; ++CP) {
			if (CountIndex < CPMatches[CP].Count) {
				(void)fprintf(PokemonOutputFile, ",%u", CPMatches[CP].ColumnData[CountIndex].CPStringIndex);
				//(void)fprintf(PokemonOutputFile, ",%3u%o%2u%X%X%X", CPMatches[CP].ColumnData[CountIndex].Struct.Dex, CPMatches[CP].ColumnData[CountIndex].Struct.Form, CPMatches[CP].ColumnData[CountIndex].Struct.Level, CPMatches[CP].ColumnData[CountIndex].Struct.AttackIV, CPMatches[CP].ColumnData[CountIndex].Struct.DefenseIV, CPMatches[CP].ColumnData[CountIndex].Struct.HPIV);
			}
			else {
				(void)fprintf(PokemonOutputFile, ",");
			}
		}
		(void)fprintf(PokemonOutputFile, "\n");
	}
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		free(CPMatches[CP].ColumnData);
	}
	free(CPMatches);
	(void)fclose(PokemonOutputFile);
#elif CalcMode == XLSXMode
	FILE* SharedStrings = fopen("F:\\My Programming Stuff Expansion\\ExcelFileTest\\xl\\sharedStrings.xml", "w+");
	(void)fprintf(SharedStrings, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" count=\"%u\" uniqueCount=\"%u\">", CPCount, CPCount);
	for (CP = 0; CP < CPCount; ++CP) {
		CrappyPrintf("%u\r", CP)
		(void)fprintf(SharedStrings, "<si><t>%3u%o%2u%X%X%X</t></si>", CPList[CP].Struct.Dex, CPList[CP].Struct.Form, CPList[CP].Struct.Level, CPList[CP].Struct.AttackIV, CPList[CP].Struct.DefenseIV, CPList[CP].Struct.HPIV);
	}
	free(CPList);
	(void)fprintf(SharedStrings, "</sst>");
	(void)fclose(SharedStrings);
	uint_fast32_t MaxCount = 0;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		if (CPMatches[CP].Count) {
			if (CPMatches[CP].Count > MaxCount) {
				MaxCount = CPMatches[CP].Count;
			}
		}
	}
	char* EndMySuffering = SaferMalloc(char, EndMySuffering, MaxCP * 4)
	FILE * AAAAAAAAH = fopen("FML.bin", "rb");
	(void)fread(EndMySuffering, MaxCP * 4, sizeof(char), AAAAAAAAH);
	(void)fclose(AAAAAAAAH);
	CrappyPrintf("\nRowCount:\n%u\n", MaxCount - 1)
	FILE* PokemonOutputFile = fopen("F:\\My Programming Stuff Expansion\\ExcelFileTest\\xl\\worksheets\\sheet1.xml", "w+");
	(void)fprintf(PokemonOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" mc:Ignorable=\"x14ac\" xmlns:x14ac=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac\"><dimension ref=\"A1:C4\"/><sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"/></sheetViews><sheetFormatPr defaultRowHeight=\"15\" x14ac:dyDescent=\"0.25\"/><sheetData>");
	for (uint_fast32_t CountIndex = 0; CountIndex < MaxCount; ++CountIndex) {
		CrappyPrintf("%u\r", CountIndex)
		(void)fprintf(PokemonOutputFile, "<row r=\"%u\" spans=\"1:3\" x14ac:dyDescent=\"0.25\">", CountIndex);
		for (CP = MinCP; CP < MaxCP; ++CP) {
			if (CountIndex < CPMatches[CP].Count) {
				(void)fprintf(PokemonOutputFile, "<c r=\"%s%u1\" t=\"s\"><v>%u</v></c>", &EndMySuffering[CP * 4], CountIndex, CPMatches[CP].ColumnData[CountIndex].CPStringIndex);
			}
		}
		(void)fprintf(PokemonOutputFile, "</row>");
	}
	free(EndMySuffering);
	(void)fprintf(PokemonOutputFile, "</sheetData><pageMargins left=\"0.7\" right=\"0.7\" top=\"0.75\" bottom=\"0.75\" header=\"0.3\" footer=\"0.3\"/></worksheet>");
	(void)fclose(PokemonOutputFile);
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		free(CPMatches[CP].ColumnData);
	}
	free(CPMatches);
#elif CalcMode == ODSMode
	FILE* PokemonOutputFile = fopen(".\\PokemonCPOutput.xml", "w+");
	(void)fprintf(PokemonOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n<office:document-content xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\" xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\" xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\" xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\" xmlns:of=\"urn:oasis:names:tc:opendocument:xmlns:of:1.2\" office:version=\"1.2\"><office:font-face-decls><style:font-face style:name=\"Calibri\" svg:font-family=\"Calibri\"/></office:font-face-decls><office:automatic-styles><style:style style:name=\"ce1\" style:family=\"table-cell\" style:parent-style-name=\"Default\" style:data-style-name=\"N0\"/><style:style style:name=\"co1\" style:family=\"table-column\"><style:table-column-properties fo:break-before=\"auto\" style:column-width=\"1.69333333333333cm\"/></style:style><style:style style:name=\"ro1\" style:family=\"table-row\"><style:table-row-properties style:row-height=\"15pt\" style:use-optimal-row-height=\"true\" fo:break-before=\"auto\"/></style:style><style:style style:name=\"ta1\" style:family=\"table\" style:master-page-name=\"mp1\"><style:table-properties table:display=\"true\" style:writing-mode=\"lr-tb\"/></style:style></office:automatic-styles><office:body><office:spreadsheet><table:calculation-settings table:case-sensitive=\"false\" table:search-criteria-must-apply-to-whole-cell=\"true\" table:use-wildcards=\"true\" table:use-regular-expressions=\"false\" table:automatic-find-labels=\"false\"/><table:table table:name=\"OutputSheet\" table:style-name=\"ta1\">");
	PokemonOutputStruct** PokemonOutput = SaferCalloc(PokemonOutputStruct*, PokemonOutput, MaxCP, sizeof(PokemonOutputStruct*))
	PokemonOutputData* OutputRef = NULL, * OutputRef2;
	uint_fast32_t MaxCount = 0;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		CrappyPrintf("%u\r", CP)
		if (CPMatches[CP].Count) {
			if (CPMatches[CP].Count > MaxCount) {
				MaxCount = CPMatches[CP].Count;
			}
#pragma warning(suppress:6011)
			PokemonOutput[CP] = SaferCalloc(PokemonOutputStruct, PokemonOutput[CP], CPMatches[CP].Count, sizeof(PokemonOutputStruct))
			OutputRef = CPMatches[CP].StartOfColumn;
			for (uint_fast32_t CountIndex = 0; CountIndex < CPMatches[CP].Count; ++CountIndex) {
#pragma warning(suppress:6001)
				PokemonOutput[CP][CountIndex] = OutputRef->Data.Struct;
				OutputRef2 = OutputRef;
				OutputRef = OutputRef->NextData;
				free(OutputRef2);
			}
		}
	}
	CrappyPrintf("\nMaxCount: %u\nCount:\n", MaxCount)
	for (CP = MinCP; CP <= MaxCP; ++CP) {
		(void)fprintf(PokemonOutputFile, "");
	}
	for (uint_fast32_t CountIndex = 0; CountIndex < MaxCount; ++CountIndex) {
		CrappyPrintf("%u\r", CountIndex)
		if (CountIndex < CPMatches[0].Count) {
			(void)fprintf(PokemonOutputFile, "%3u%o%2u%X%X%X", PokemonOutput[0][CountIndex].Dex, PokemonOutput[0][CountIndex].Form, PokemonOutput[0][CountIndex].Level, PokemonOutput[0][CountIndex].AttackIV, PokemonOutput[0][CountIndex].DefenseIV, PokemonOutput[0][CountIndex].HPIV);
		}
		for (CP = MinCP; CP < MaxCP; ++CP) {
			if (CountIndex < CPMatches[CP].Count) {
				(void)fprintf(PokemonOutputFile, ",%3u%o%2u%X%X%X", PokemonOutput[CP][CountIndex].Dex, PokemonOutput[CP][CountIndex].Form, PokemonOutput[CP][CountIndex].Level, PokemonOutput[CP][CountIndex].AttackIV, PokemonOutput[CP][CountIndex].DefenseIV, PokemonOutput[CP][CountIndex].HPIV);
			}
			else {
				(void)fprintf(PokemonOutputFile, ",");
			}
		}
		(void)fprintf(PokemonOutputFile, "\n");
	}
	free(CPMatches);
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		free(PokemonOutput[CP]);
	}
	free(PokemonOutput);
	(void)fclose(PokemonOutputFile);
#endif
	(void)printf("\nDone");
}