# SelfLogger

#### Self Logger (cout-like) For All Our Repos

## How To Use:

<h3>1. Clone this repo like a submodule</h3>
<h3>2. Somewhere in your code define the following lines of code to set up log files and their type of Level Debug</h3>

```c++
#if __has_include("logger.h")
CppLogger::registerTarget(new FileLoggerTarget("./render-system-all.log", LogLevel::LOG_LEVEL_DEBUG));
CppLogger::registerTarget(new FileLoggerTarget("./render-system-err.log", LogLevel::LOG_LEVEL_ERROR));
#endif
```

<h3>3. And then use it like this:</h3>
<h4>With Params (Have Postfix Letter "F")</h4>

```c++
Logger_Fatal_F("Unexpected arguments: %s", renderTypeArg.c_str())
```

<h4>Or</h4>

```c++
Logger_Warn("Build demo scene")
```

<h3>4. Profit!</h3>
