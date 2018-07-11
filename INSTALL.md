
# Installing nxxm::gh
gh can be installed in three ways :

* [Copy paste in your project](#copy-paste-in-your-project)
* [Install](#install)
* [Install without dependencies](#install-without-dependencies)

## Copy paste in your project
1. Copy this folder in your project
2. Add the following to your include path :

### GCC / Clang / MSVC
```
-I gh/ \
-I gh/deps/aantron/better-enums/ \
-I gh/deps/boostorg/algorithm/include/ \
-I gh/deps/boostorg/array/include/ \
-I gh/deps/boostorg/asio/include/ \
-I gh/deps/boostorg/assert/include/ \
-I gh/deps/boostorg/atomic/include/ \
-I gh/deps/boostorg/beast/include/ \
-I gh/deps/boostorg/bind/include/ \
-I gh/deps/boostorg/chrono/include/ \
-I gh/deps/boostorg/concept_check/include/ \
-I gh/deps/boostorg/config/include/ \
-I gh/deps/boostorg/container/include/ \
-I gh/deps/boostorg/container_hash/include/ \
-I gh/deps/boostorg/context/include/ \
-I gh/deps/boostorg/core/include/ \
-I gh/deps/boostorg/coroutine/include/ \
-I gh/deps/boostorg/date_time/include/ \
-I gh/deps/boostorg/detail/include/ \
-I gh/deps/boostorg/endian/include/ \
-I gh/deps/boostorg/exception/include/ \
-I gh/deps/boostorg/filesystem/include/ \
-I gh/deps/boostorg/foreach/include/ \
-I gh/deps/boostorg/function/include/ \
-I gh/deps/boostorg/function_types/include/ \
-I gh/deps/boostorg/functional/include/ \
-I gh/deps/boostorg/fusion/include/ \
-I gh/deps/boostorg/integer/include/ \
-I gh/deps/boostorg/intrusive/include/ \
-I gh/deps/boostorg/io/include/ \
-I gh/deps/boostorg/iostreams/include/ \
-I gh/deps/boostorg/iterator/include/ \
-I gh/deps/boostorg/lambda/include/ \
-I gh/deps/boostorg/lexical_cast/include/ \
-I gh/deps/boostorg/locale/include/ \
-I gh/deps/boostorg/math/include/ \
-I gh/deps/boostorg/move/include/ \
-I gh/deps/boostorg/mpl/include/ \
-I gh/deps/boostorg/numeric_conversion/include/ \
-I gh/deps/boostorg/optional/include/ \
-I gh/deps/boostorg/phoenix/include/ \
-I gh/deps/boostorg/pool/include/ \
-I gh/deps/boostorg/predef/include/ \
-I gh/deps/boostorg/preprocessor/include/ \
-I gh/deps/boostorg/proto/include/ \
-I gh/deps/boostorg/random/include/ \
-I gh/deps/boostorg/range/include/ \
-I gh/deps/boostorg/ratio/include/ \
-I gh/deps/boostorg/rational/include/ \
-I gh/deps/boostorg/regex/include/ \
-I gh/deps/boostorg/serialization/include/ \
-I gh/deps/boostorg/smart_ptr/include/ \
-I gh/deps/boostorg/spirit/include/ \
-I gh/deps/boostorg/static_assert/include/ \
-I gh/deps/boostorg/system/include/ \
-I gh/deps/boostorg/test/include/ \
-I gh/deps/boostorg/thread/include/ \
-I gh/deps/boostorg/throw_exception/include/ \
-I gh/deps/boostorg/timer/include/ \
-I gh/deps/boostorg/tokenizer/include/ \
-I gh/deps/boostorg/tti/include/ \
-I gh/deps/boostorg/tuple/include/ \
-I gh/deps/boostorg/type_index/include/ \
-I gh/deps/boostorg/type_traits/include/ \
-I gh/deps/boostorg/typeof/include/ \
-I gh/deps/boostorg/utility/include/ \
-I gh/deps/boostorg/variant/include/ \
-I gh/deps/boostorg/winapi/include/ \
-I gh/deps/cpp-pre/json/ \
-I gh/deps/cpp-pre/type_traits/ \
-I gh/deps/daminetreg/js-bind/ \
-I gh/deps/daminetreg/xxhr/ \
-I gh/deps/nlohmann/json/include/ \
```

## Install
Install the library
```sh
mkdir build/ && cd build/
cmake ..
make install
```

You can tweak destination with `-DCMAKE_INSTALL_PREFIX=/install/path` to change the default `/usr/local`.

## If you use CMake :
```cmake
find_package(gh REQUIRED)
add_executable(app your_code.cpp)
target_link_libraries(app nxxm::gh)
```

## If you don't use CMake : 
Add to your include path : 
```
INSTALL_PREFIX/include/gh/deps/
INSTALL_PREFIX/include/
```

## Install without dependencies
Install the library
```sh
mkdir build/ && cd build/
cmake .. -DINGLUED=OFF
make install
```

This library was built with [`#inglued <>`](https://github.com/header-only/inglued) therefore it forwards dependencies with it, but you might prefer the use of your own deps, therefore simply disable them via `INGLUED=OFF`.
