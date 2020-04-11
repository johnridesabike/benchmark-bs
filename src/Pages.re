module About = {
  [@react.component]
  let make = () =>
    <article>
      <h1> "About Benchmark.bs.js"->React.string </h1>
      <p>
        {j|This is a web-app for measuring and comparing the performance speed
           of Reason functions when compiled with BuckleScript. It's powered by |j}
        ->React.string
        <a href="https://benchmarkjs.com/"> "Benchmark.js"->React.string </a>
        "."->React.string
      </p>
      <p>
        {j|Understanding performance isn't easy, and is made more complicated in
           Reason. Functions that may be performant on native Reason, but much
           slower when compiled to JavaScript. Many libraries use functions that
           appear the same, but may function differently depending on their
           implementation details.|j}
        ->React.string
      </p>
      <p>
        {j|To add your own benchmark tests, you will need to fork and clone this
           project's git repository. Follow the instructions in its README.|j}
        ->React.string
      </p>
    </article>;
};
