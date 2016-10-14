@0xe196d5513ec00f45;

struct Copy {
	origin @0 :UInt32;
	dest @1 :UInt32;
	length @2 :UInt32;
}

struct Create {
	dest @0 :UInt32;
	data @1 :Data;
}

struct Patch {
	hash @0 :Data;
	length @1 :UInt32;
	copyList @2 :List(Copy);
	createList @3 :List(Create);
}
