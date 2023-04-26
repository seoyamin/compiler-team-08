/*
* tn.h – token type 
* progrmmer – 김민서, 정은비, 최민교
* date - 04/26/2023
*/

enum tokentypes {
	TEOF, TCONST, TELSE, TIF, TINT, TRETURN,
	TVOID, TWHILE, TADD, TSUB, TMUL,
	TDIV, TMOD, TASSIGN, TADDASSIGN, TSUBASSIGN,
	TMULASSIGN, TDIVASSIGN, TMODASSIGN, TNOT, TAND,
	TOR, TEQUAL, TNOTEQU, TLESS, TGREAT,
	TLESSE, TGREATE, TINC, TDEC, TLEFTPAR,
	TRIGHTPAR, TCOLON, TLEFTBRACE, TRIGHTBRACE, TLEFTBRACKET,
	TRIGHTBRACKET, TSEMI, TNUMBER, TIDENT, TERROR
};
