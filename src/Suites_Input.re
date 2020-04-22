module DestructureTuple = {
  let two = (1, 2);
  let eight = (1, 2, 3, 4, 5, 6, 7, 8);
};

module ImmutableObjUpdate = {
  type tenFields = {
    a: int,
    b: string,
    c: int,
    d: string,
    e: int,
    f: string,
    g: int,
    h: string,
    i: int,
    j: string,
  };
  let tenFields = {
    a: 1,
    b: "b",
    c: 3,
    d: "d",
    e: 5,
    f: "f",
    g: 7,
    h: "h",
    i: 9,
    j: "j",
  };
};
