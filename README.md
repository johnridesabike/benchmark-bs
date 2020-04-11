# Benchmark.bs.js

To add your own benchmarks, start by forking or cloning this git repository.
Then, open [Suites.re](src/Suites.re) to add your own code.

```reason
module ArrayAccess = {
  let name = "Array access";
  let arr = Belt.Array.make(100, "a");
  let setup = {j|let arr = Belt.Array.make(100, "a");|j};
  let benchmarks = [|
    {
      name: "Array.get",
      f: (.) => arr[50]->ignore,
      code: "arr[50];"
    },
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
```

Namespacing your benchmark with a module isn't necessary, but helps keep it
organized.

To make your code accessible on the web interface, you need to register it in
the `Routes` module (located at the bottom of the `Suites` module).

```reason
module Routes = {
  type item = {
    suite: t,
    url: string,
  };
  type key =
    | Strings
    | ListArray
    | MapsGet
    | MapsSet
    | ArrayAccess; /* <- Add a variant here to identify it. */

  let map =
    fun
    | Strings => {suite: Strings.suite, url: "concat-strings"}
    | ListArray => {suite: ListArray.suite, url: "list-array"}
    | MapsGet => {suite: Maps.getting, url: "maps-get"}
    | MapsSet => {suite: Maps.setting, url: "maps-set"}
    /* Add the suite and the url here. */
    | ArrayAccess => {suite: ArrayAccess.suite, url: "array-access"};

  let fromUrl =
    fun
    | "concat-strings" => Some(Strings)
    | "list-array" => Some(ListArray)
    | "maps-get" => Some(MapsGet)
    | "maps-set" => Some(MapsSet)
    | "array-access" => Some(ArrayAccess) /* <- Add the same url here. */
    | _ => None;

  /* Add it to this array to be listed on the main menu. */
  let routes = [|Strings, ListArray, MapsGet, MapsSet, ArrayAccess|];
};
```
