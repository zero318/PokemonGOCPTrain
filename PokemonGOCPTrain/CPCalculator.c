#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MinPokemon 1
#define MaxPokemon 964
#define MinLevel 1
#define MaxLevel 89
#define MinIV 0
#define MaxIV 15
#define MinCP 1
#define MaxCP 10403
#define MinCPM 1
#define MaxCPM 45
#define BinaryMode 0
#define CSVMode 1
#define ODSMode 2
#define Mode CSVMode

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

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
	struct PokemonOutputStruct Struct;
	uint8_t OutChars[4];
} PokemonOutputUnion;

typedef struct PokemonOutputData {
	union PokemonOutputUnion Data;
	struct PokemonOutputData* NextData;
} PokemonOutputData;

typedef struct {
	uint_fast32_t Count;
	struct PokemonOutputData* StartOfColumn;
	struct PokemonOutputData* CurrentData;
} CPMatchesData;
#pragma pack(pop)

void main() {
	(void)printf("Pokemon GO CP Combination Calculator\nPokemon Count:\n");
	FILE *PokemonStatsFile = fopen("F:\\My Programming Stuff Expansion\\PokemonStats.dat", "rb");
	double CPMDoublesSquared[MaxLevel];
	uint_fast8_t CPM = 0, Level = 0;
	{//Lol, I don't need CPMFloats after this
		CPMFloat CPMFloats[MaxCPM];
		(void)fread(&CPMFloats->InChars, MaxCPM, sizeof(float), PokemonStatsFile);
		for (Level; Level < MaxLevel; ++Level) {
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
	PokemonInputData* PokemonData = (PokemonInputData*)calloc(MaxPokemon, sizeof(PokemonInputData));
	if (!PokemonData) {
		printf("calloc of count %ul and size %zu failed!\n", MaxPokemon, sizeof(PokemonInputData));
		exit(1);
	}
	(void)fread(PokemonData, MaxPokemon, 6, PokemonStatsFile);
	(void)fclose(PokemonStatsFile);
	CPMatchesData* CPMatches = (CPMatchesData*)calloc(MaxCP, sizeof(CPMatchesData));
	if (!CPMatches) {
		printf("calloc of count %ul and size %zu failed!\n", MaxCP, sizeof(CPMatchesData));
		exit(1);
	}
	CPMatchesData *CPMatchRef;
	for (uint_fast16_t Pokemon = 0; Pokemon < MaxPokemon; ++Pokemon) {
		(void)printf("%u\r", Pokemon);
		for (Level = MinLevel; Level <= MaxLevel; ++Level) {
			for (uint_fast8_t AttackIV = MinIV; AttackIV < MaxIV; ++AttackIV) {
				for (uint_fast8_t DefenseIV = MinIV; DefenseIV < MaxIV; ++DefenseIV) {
					for (uint_fast8_t HPIV = MinIV; HPIV < MaxIV; ++HPIV) {
#pragma warning(suppress:26451)
						CPMatchRef = &CPMatches[(uint_fast16_t)(((PokemonData[Pokemon].BaseAttack + AttackIV) * sqrt(PokemonData[Pokemon].BaseDefense + DefenseIV) * sqrt(PokemonData[Pokemon].BaseHP + HPIV) * CPMDoublesSquared[Level - 1]) / 10) - 1];
						++(CPMatchRef->Count);
						if (CPMatchRef->CurrentData) {
							CPMatchRef->CurrentData->NextData = (struct PokemonOutputData*)malloc(sizeof(struct PokemonOutputData));
							if (!CPMatchRef->CurrentData->NextData) {//If null pointer
								printf("malloc of size %zu failed!\n", sizeof(struct PokemonOutputData));
								exit(1);
							}
							CPMatchRef->CurrentData = CPMatchRef->CurrentData->NextData;
						}
						else {
							CPMatchRef->StartOfColumn = (struct PokemonOutputData*)malloc(sizeof(struct PokemonOutputData));
							if (!CPMatchRef->StartOfColumn) {//If null pointer
								printf("malloc of size %zu failed!\n", sizeof(struct PokemonOutputData));
								exit(1);
							}
							CPMatchRef->CurrentData = CPMatchRef->StartOfColumn;
						}
#pragma warning(suppress:6011)
						CPMatchRef->CurrentData->Data.Struct.Dex = PokemonData[Pokemon].Dex;
						CPMatchRef->CurrentData->Data.Struct.Form = PokemonData[Pokemon].Form;
						CPMatchRef->CurrentData->Data.Struct.Level = Level;
						CPMatchRef->CurrentData->Data.Struct.AttackIV = AttackIV;
						CPMatchRef->CurrentData->Data.Struct.DefenseIV = DefenseIV;
						CPMatchRef->CurrentData->Data.Struct.HPIV = HPIV;
					}
				}
			}
		}
	}
	free(PokemonData);
	(void)printf("\nCPCount:\n");
	uint_fast16_t CP;
	if (Mode == BinaryMode) {
		FILE* PokemonOutputFile = fopen("F:\\My Programming Stuff Expansion\\PokemonCPOutput.dat", "w+b");
		PokemonOutputUnion ColumnSpacer = { .Struct.HPIV = 0, .Struct.DefenseIV = 0, .Struct.AttackIV = 0, .Struct.Level = 0, .Struct.Form = 0, .Struct.Dex = 0 };
		PokemonOutputData* OutputRef = NULL, * OutputRef2;
		for (CP = MinCP - 1; CP < MaxCP; ++CP) {
			(void)printf("%u\r", CP);
			if (CPMatches[CP].StartOfColumn) {
				OutputRef = CPMatches[CP].StartOfColumn;
				for (uint_fast32_t PokemonOutput = 0; PokemonOutput < CPMatches[CP].Count; ++PokemonOutput) {
#pragma warning(suppress:6001)
					(void)fwrite(OutputRef->Data.OutChars, 1, 4, PokemonOutputFile);
					OutputRef2 = OutputRef;
					OutputRef = OutputRef->NextData;
					free(OutputRef2);
				}
				(void)fwrite(ColumnSpacer.OutChars, 1, 4, PokemonOutputFile);
			}
		}
		free(CPMatches);
		(void)fclose(PokemonOutputFile);
	}
	else if (Mode == CSVMode) {
		FILE* PokemonOutputFile = fopen("F:\\My Programming Stuff Expansion\\PokemonCPOutput.csv", "w+");
		PokemonOutputStruct** PokemonOutput = (PokemonOutputStruct**)calloc(MaxCP, sizeof(PokemonOutputStruct*));
		if (!PokemonOutput) {
			printf("calloc of count %ul and size %zu failed!\n", MaxCP, sizeof(PokemonOutputStruct*));
			exit(1);
		}
		PokemonOutputData* OutputRef = NULL, * OutputRef2;
		uint_fast32_t MaxCount = 0;
		for (CP = MinCP - 1; CP < MaxCP; ++CP) {
			(void)printf("%u\r", CP);
			if (CPMatches[CP].Count) {
				if (CPMatches[CP].Count > MaxCount) {
					MaxCount = CPMatches[CP].Count;
				}
#pragma warning(suppress:6011)
				PokemonOutput[CP] = (PokemonOutputStruct*)calloc(CPMatches[CP].Count, sizeof(PokemonOutputStruct));
				if (!PokemonOutput[CP]) {
					printf("calloc of count %ul and size %zu failed!\n", CPMatches[CP].Count, sizeof(PokemonOutputStruct));
					exit(1);
				}
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
		(void)printf("\nMaxCount: %u\nCount:\n", MaxCount);
		for (CP = MinCP; CP <= MaxCP; ++CP) {
			(void)fprintf(PokemonOutputFile, "");
		}
		for (uint_fast32_t CountIndex = 0; CountIndex < MaxCount; ++CountIndex) {
			(void)printf("%u\r", CountIndex);
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
	}/*
	else if (Mode == ODSMode) {
		FILE* PokemonOutputFile = fopen("F:\\My Programming Stuff Expansion\\PokemonCPOutput.xml", "w+");
		(void)fprintf(PokemonOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n<office:document-content xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\" xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\" xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\" xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\" xmlns:of=\"urn:oasis:names:tc:opendocument:xmlns:of:1.2\" office:version=\"1.2\"><office:font-face-decls><style:font-face style:name=\"Calibri\" svg:font-family=\"Calibri\"/></office:font-face-decls><office:automatic-styles><style:style style:name=\"ce1\" style:family=\"table-cell\" style:parent-style-name=\"Default\" style:data-style-name=\"N0\"/><style:style style:name=\"co1\" style:family=\"table-column\"><style:table-column-properties fo:break-before=\"auto\" style:column-width=\"1.69333333333333cm\"/></style:style><style:style style:name=\"ro1\" style:family=\"table-row\"><style:table-row-properties style:row-height=\"15pt\" style:use-optimal-row-height=\"true\" fo:break-before=\"auto\"/></style:style><style:style style:name=\"ta1\" style:family=\"table\" style:master-page-name=\"mp1\"><style:table-properties table:display=\"true\" style:writing-mode=\"lr-tb\"/></style:style></office:automatic-styles><office:body><office:spreadsheet><table:calculation-settings table:case-sensitive=\"false\" table:search-criteria-must-apply-to-whole-cell=\"true\" table:use-wildcards=\"true\" table:use-regular-expressions=\"false\" table:automatic-find-labels=\"false\"/><table:table table:name=\"OutputSheet\" table:style-name=\"ta1\">");
		PokemonOutputStruct** PokemonOutput = (PokemonOutputStruct**)calloc(MaxCP, sizeof(PokemonOutputStruct*));
		if (!PokemonOutput) {
			printf("calloc of count %ul and size %zu failed!\n", MaxCP, sizeof(PokemonOutputStruct*));
			exit(1);
		}
		PokemonOutputData* OutputRef = NULL, * OutputRef2;
		uint_fast32_t MaxCount = 0;
		for (CP = MinCP - 1; CP < MaxCP; ++CP) {
			(void)printf("%u\r", CP);
			if (CPMatches[CP].Count) {
				if (CPMatches[CP].Count > MaxCount) {
					MaxCount = CPMatches[CP].Count;
				}
#pragma warning(suppress:6011)
				PokemonOutput[CP] = (PokemonOutputStruct*)calloc(CPMatches[CP].Count, sizeof(PokemonOutputStruct));
				if (!PokemonOutput[CP]) {
					printf("calloc of count %ul and size %zu failed!\n", CPMatches[CP].Count, sizeof(PokemonOutputStruct));
					exit(1);
				}
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
		(void)printf("\nMaxCount: %u\nCount:\n", MaxCount);
		for (CP = MinCP; CP <= MaxCP; ++CP) {
			(void)fprintf(PokemonOutputFile, "");
		}
		for (uint_fast32_t CountIndex = 0; CountIndex < MaxCount; ++CountIndex) {
			(void)printf("%u\r", CountIndex);
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
	}*/
	(void)printf("\nDone");
}