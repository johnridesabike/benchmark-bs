type benchmark = {
  name: string,
  code: string,
  f: (. unit) => unit,
};

type t = {
  name: string,
  setup: string,
  benchmarks: array(benchmark),
};

module Strings = {
  [@bs.module "nanoid"] external randomString: unit => string = "nanoid";
  let strings = Belt.List.makeBy(100, _ => randomString());
  let stringsArr = Belt.List.toArray(strings);

  let setup = {j|[@bs.module "nanoid"] external randomString: unit => string = "nanoid";
let strings = Belt.List.makeBy(100, _ => randomString());
let stringsArr = Belt.List.toArray(strings);|j};

  let benchmarks = [|
    {
      name: "Pervasives.(++)",
      f:
        (.) => {
          let rec loop = (acc, l) =>
            switch (l) {
            | [] => acc
            | [a, ...l] => loop(a ++ acc, l)
            };
          loop("", strings)->ignore;
        },
      code: {j|let rec loop = (acc, l) =>
  switch (l) {
  | [] => acc
  | [a, ...l] => loop(a ++ acc, l)
  };
loop("", strings);|j},
    },
    {
      name: "String.concat",
      f: (.) => strings |> String.concat("") |> ignore,
      code: {j|strings |> String.concat("");|j},
    },
    {
      name: "Js.Array.joinWith",
      f: (.) => stringsArr |> Js.Array.joinWith("") |> ignore,
      code: {j|stringsArr |> Js.Array.joinWith("");|j},
    },
  |];

  let name = "Concatenate 100 strings";

  let suite = {setup, name, benchmarks};
};

module ListArray = {
  let name = "List vs Array: Map 100 items";

  let ints = Belt.List.makeBy(100, i => i);
  let intsArr = Belt.List.toArray(ints);

  let setup = {j|let ints = Belt.List.makeBy(100, i => i);
let intsArr = Belt.List.toArray(ints);|j};

  let benchmarks = [|
    {
      name: "List.map",
      f: (.) => ints |> List.map(x => x + 1) |> ignore,
      code: "ints |> List.map(x => x + 1)",
    },
    {
      name: "Belt.List.map",
      f: (.) => ints->Belt.List.map(x => x + 1)->ignore,
      code: "ints->Belt.List.map(x => x + 1)",
    },
    {
      name: "Belt.List.mapU",
      f: (.) => ints->Belt.List.mapU((. x) => x + 1)->ignore,
      code: "ints->Belt.List.mapU((. x) => x + 1)",
    },
    {
      name: "Array.map",
      f: (.) => intsArr |> Array.map(x => x + 1) |> ignore,
      code: "intsArr |> Array.map(x => x + 1)",
    },
    {
      name: "Belt.Array.map",
      f: (.) => intsArr->Belt.Array.map(x => x + 1)->ignore,
      code: "intsArr->Belt.Array.map(x => x + 1)",
    },
    {
      name: "Belt.Array.mapU",
      f: (.) => intsArr->Belt.Array.mapU((. x) => x + 1)->ignore,
      code: "intsArr->Belt.Array.mapU((. x) => x + 1)",
    },
    {
      name: "Js.Array.map",
      f: (.) => intsArr |> Js.Array.map(x => x + 1) |> ignore,
      code: "intsArr |> Js.Array.map(x => x + 1)",
    },
  |];

  let map = {name, setup, benchmarks};

  let name = "List vs Array: access first item";

  let benchmarks = [|
    {
      name: "pattern match",
      f:
        (.) => {
          let _ =
            switch (ints) {
            | [x, ..._] => Some(x)
            | [] => None
            };
          ();
        },
      code: {j|switch (ints) {
| [x, ..._] => Some(x)
| [] => None
};|j},
    },
    {
      name: "List.hd",
      f: (.) => ints->List.hd->ignore,
      code: "ints->List.hd->ignore;",
    },
    {
      name: "Belt.List.head",
      f: (.) => ints->Belt.List.head->ignore,
      code: "ints->Belt.List.head->ignore;",
    },
    {
      name: "Belt.List.headExn",
      f: (.) => ints->Belt.List.headExn->ignore,
      code: "ints->Belt.List.headExn->ignore;",
    },
    {
      name: "Array.get",
      f: (.) => intsArr[0]->ignore,
      code: "intsArr[0]->ignore;",
    },
    {
      name: "Belt.Array.get",
      f: (.) => Belt.(intsArr[0])->ignore,
      code: "Belt.(intsArr[0])->ignore;",
    },
    {
      name: "Belt.Array.getExn",
      f: (.) => intsArr->Belt.Array.getExn(0)->ignore,
      code: "intsArr->Belt.Array.getExn(0)->ignore;",
    },
  |];

  let head = {setup, name, benchmarks};
};

module Maps = {
  let name = "Maps: Getting a key from a map with 100 items";

  [@bs.module "nanoid"] external randomString: unit => string = "nanoid";

  module JsMap = {
    type t('key, 'value);
    [@bs.new]
    external fromArray: array(('key, 'value)) => t('key, 'value) = "Map";
    [@bs.new] external copy: t('key, 'value) => t('key, 'value) = "Map";
    [@bs.send] external set: (t('key, 'value), 'key, 'value) => unit = "set";
    [@bs.send] external get: (t('key, 'value), 'key) => 'value = "get";
  };

  module StringMap = Map.Make(String);
  let strings = Belt.Array.makeBy(100, _ => (randomString(), randomString()));
  let dict = Js.Dict.fromArray(strings);
  let jsMap = JsMap.fromArray(strings);
  let hashmap = Belt.HashMap.String.fromArray(strings);
  let map = Belt.Map.String.fromArray(strings);
  let stdlibMap =
    Belt.Array.reduce(strings, StringMap.empty, (acc, (k, v)) =>
      StringMap.add(k, v, acc)
    );
  let (key, _) = strings[0];

  let setup = {j|[@bs.module "nanoid"] external randomString: unit => string = "nanoid";

module JsMap = {
  type t('key, 'value);
  [@bs.new] external fromArray: array(('key, 'value)) => t('key, 'value) = "Map";
  [@bs.new] external copy: t('key, 'value) => t('key, 'value) = "Map";
  [@bs.send] external set: (t('key, 'value), 'key, 'value) => unit = "set";
  [@bs.send] external get: (t('key, 'value), 'key) => 'value = "get";
};

module StringMap = Map.Make(String);
let strings = Belt.Array.makeBy(100, _ => (randomString(), randomString()));
let dict = Js.Dict.fromArray(strings);
let jsMap = JsMap.fromArray(strings);
let hashmap = Belt.HashMap.String.fromArray(strings);
let map = Belt.Map.String.fromArray(strings);
let stdlibMap =
  Belt.Array.reduce(strings, StringMap.empty, (acc, (k, v)) =>
    StringMap.add(k, v, acc)
  );
let (key, _) = strings[0];|j};

  let benchmarks = [|
    {
      name: "Map.Make(String).find",
      f: (.) => stdlibMap |> StringMap.find(key) |> ignore,
      code: "stdlibMap |> StringMap.find(key)",
    },
    {
      name: "Belt.Map.String.get",
      f: (.) => map->Belt.Map.String.get(key)->ignore,
      code: "map->Belt.Map.String.get(key)",
    },
    {
      name: "Belt.HashMap.String.get",
      f: (.) => hashmap->Belt.HashMap.String.get(key)->ignore,
      code: "hashMap->Belt.HashMap.String.get(key)",
    },
    {
      name: "Js.Dict.get",
      f: (.) => dict->Js.Dict.get(key)->ignore,
      code: "dict->Js.Dict.get(key)",
    },
    {
      name: "JsMap get",
      f: (.) => jsMap->JsMap.get(key)->ignore,
      code: "jsMap->JsMap.get(key)",
    },
  |];

  let getting = {name, setup, benchmarks};

  let name = "Maps: Immutably setting a key from a map with 100 items";

  [@bs.val] [@bs.scope "Object"]
  external assign: (Js.Dict.t('a), Js.Dict.t('a)) => Js.Dict.t('a) =
    "assign";
  let cloneDict = d => assign(Js.Dict.empty(), d);

  let setup =
    setup
    ++ {j|

[@bs.val] [@bs.scope "Object"]
external assign: (Js.Dict.t('a), Js.Dict.t('a)) => Js.Dict.t('a) =
  "assign";
let cloneDict = d => assign(Js.Dict.empty(), d);|j};

  let benchmarks = [|
    {
      name: "Map.Make(String).add",
      f: (.) => stdlibMap |> StringMap.add(key, "a") |> ignore,
      code: {j|stdlibMap |> StringMap.add(key, "a")|j},
    },
    {
      name: "Belt.Map.String.set",
      f: (.) => map->Belt.Map.String.set(key, "a")->ignore,
      code: {j|map->Belt.Map.String.set(key, "a")|j},
    },
    {
      name: {j|Belt.HashMap.String.(copy->set)|j},
      f:
        (.) =>
          hashmap
          ->Belt.HashMap.String.copy
          ->Belt.HashMap.String.set(key, "a")
          ->ignore,
      code: {j|hashMap
->Belt.Hashmap.String.copy
->Belt.HashMap.String.set(key, "a")|j},
    },
    {
      name: "cloneDict->Js.Dict.set",
      f: (.) => dict->cloneDict->Js.Dict.set(key, "a")->ignore,
      code: {j|dict->cloneDict->Js.Dict.get(key, "a")|j},
    },
    {
      name: "JsMap copy->set",
      f: (.) => jsMap->JsMap.copy->JsMap.set(key, "a")->ignore,
      code: {j|jsMap->JsMap.copy->JsMap.set(key, "a")|j},
    },
  |];

  let setting = {name, benchmarks, setup};
};

module ArrayAccess = {
  let name = "Array access";

  let arr = Belt.Array.make(100, "a");

  let setup = {j|let arr = Belt.Array.make(100, "a");|j};

  let benchmarks = [|
    {name: "Array.get", f: (.) => arr[50]->ignore, code: "arr[50];"},
    {
      name: "Belt.Array.get",
      f: (.) => Belt.(arr[50]->ignore),
      code: "Belt.(arr[50]);",
    },
    {
      name: "Belt.Array.getExn",
      f: (.) => arr->Belt.Array.getExn(50)->ignore,
      code: "arr->Belt.Array.getExn(50);",
    },
  |];

  let suite = {setup, name, benchmarks};
};

/* Add your own benchmark here. */

/*
  module MyBenchmark = {
    let name = "";
    let setup = "";
    let benchmarks = [|{name: "", f: (.) => (), code: "()"}|];
    let suite = {name, setup, benchmarks};
  };
 */

module Routes = {
  type item = {
    suite: t,
    url: string,
  };

  /* Register the route to your benchmark by giving it a variant here. */
  type key =
    | Strings
    | ListArrayMap
    | ListArrayHead
    | MapsGet
    | MapsSet
    | ArrayAccess;

  /* Make sure the URLs are the same in both functions! */

  let map =
    fun
    | Strings => {suite: Strings.suite, url: "concat-strings"}
    | ListArrayMap => {suite: ListArray.map, url: "list-array-map"}
    | ListArrayHead => {suite: ListArray.head, url: "list-array-head"}
    | MapsGet => {suite: Maps.getting, url: "maps-get"}
    | MapsSet => {suite: Maps.setting, url: "maps-set"}
    | ArrayAccess => {suite: ArrayAccess.suite, url: "array-access"};

  let fromUrl =
    fun
    | "concat-strings" => Some(Strings)
    | "list-array-map" => Some(ListArrayMap)
    | "list-array-head" => Some(ListArrayHead)
    | "maps-get" => Some(MapsGet)
    | "maps-set" => Some(MapsSet)
    | "array-access" => Some(ArrayAccess)
    | _ => None;

  /* The main menu uses this array to list pages. */
  let routes = [|
    Strings,
    ListArrayMap,
    ListArrayHead,
    MapsGet,
    MapsSet,
    ArrayAccess,
  |];
};
