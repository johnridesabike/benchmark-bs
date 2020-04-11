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

Be aware that BuckleScript does aggresive optimizations, so check the compiled
JavaScript to ensure that your benchmark functions are actually doing what you
expect. Sometimes BuckleScript will remove "no-op" code.

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
    | ArrayAccess; /* <- Add a variant here to identify it. */

  let map =
    fun
    /* Add the suite and the url here. */
    | ArrayAccess => {suite: ArrayAccess.suite, url: "array-access"};

  let fromUrl =
    fun
    | "array-access" => Some(ArrayAccess) /* <- Add the same url here. */
    | _ => None;

  /* Add it to this array to be listed on the main menu. */
  let routes = [|ArrayAccess|];
};
```

Build the project.

```sh
npm run build:re
```

And load the development server.

```sh
npm run dev
```

And then navigate to the URL provided in the terminal (usually
`localhost:1234`). Click on your benchmark and press "run."
