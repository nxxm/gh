
# Installing nxxm::octoxxit
octoxxit can be installed in three ways :

* [Copy paste in your project](#copy-paste-in-your-project)
* [Install](#install)
* [Install without dependencies](#install-without-dependencies)

## Copy paste in your project
1. Copy this folder in your project
2. Add the following to your include path :

### GCC / Clang / MSVC
```
-I octoxxit/ \
-I octoxxit/deps/boostorg/algorithm/include/ \
-I octoxxit/deps/boostorg/array/include/ \
-I octoxxit/deps/boostorg/asio/include/ \
-I octoxxit/deps/boostorg/assert/include/ \
-I octoxxit/deps/boostorg/atomic/include/ \
-I octoxxit/deps/boostorg/beast/include/ \
-I octoxxit/deps/boostorg/bind/include/ \
-I octoxxit/deps/boostorg/chrono/include/ \
-I octoxxit/deps/boostorg/concept_check/include/ \
-I octoxxit/deps/boostorg/config/include/ \
-I octoxxit/deps/boostorg/container/include/ \
-I octoxxit/deps/boostorg/context/include/ \
-I octoxxit/deps/boostorg/core/include/ \
-I octoxxit/deps/boostorg/coroutine/include/ \
-I octoxxit/deps/boostorg/date_time/include/ \
-I octoxxit/deps/boostorg/detail/include/ \
-I octoxxit/deps/boostorg/endian/include/ \
-I octoxxit/deps/boostorg/exception/include/ \
-I octoxxit/deps/boostorg/filesystem/include/ \
-I octoxxit/deps/boostorg/foreach/include/ \
-I octoxxit/deps/boostorg/function/include/ \
-I octoxxit/deps/boostorg/function_types/include/ \
-I octoxxit/deps/boostorg/functional/include/ \
-I octoxxit/deps/boostorg/fusion/include/ \
-I octoxxit/deps/boostorg/integer/include/ \
-I octoxxit/deps/boostorg/intrusive/include/ \
-I octoxxit/deps/boostorg/io/include/ \
-I octoxxit/deps/boostorg/iostreams/include/ \
-I octoxxit/deps/boostorg/iterator/include/ \
-I octoxxit/deps/boostorg/lambda/include/ \
-I octoxxit/deps/boostorg/lexical_cast/include/ \
-I octoxxit/deps/boostorg/locale/include/ \
-I octoxxit/deps/boostorg/math/include/ \
-I octoxxit/deps/boostorg/move/include/ \
-I octoxxit/deps/boostorg/mpl/include/ \
-I octoxxit/deps/boostorg/numeric_conversion/include/ \
-I octoxxit/deps/boostorg/optional/include/ \
-I octoxxit/deps/boostorg/phoenix/include/ \
-I octoxxit/deps/boostorg/pool/include/ \
-I octoxxit/deps/boostorg/predef/include/ \
-I octoxxit/deps/boostorg/preprocessor/include/ \
-I octoxxit/deps/boostorg/proto/include/ \
-I octoxxit/deps/boostorg/random/include/ \
-I octoxxit/deps/boostorg/range/include/ \
-I octoxxit/deps/boostorg/ratio/include/ \
-I octoxxit/deps/boostorg/rational/include/ \
-I octoxxit/deps/boostorg/regex/include/ \
-I octoxxit/deps/boostorg/serialization/include/ \
-I octoxxit/deps/boostorg/smart_ptr/include/ \
-I octoxxit/deps/boostorg/spirit/include/ \
-I octoxxit/deps/boostorg/static_assert/include/ \
-I octoxxit/deps/boostorg/system/include/ \
-I octoxxit/deps/boostorg/thread/include/ \
-I octoxxit/deps/boostorg/throw_exception/include/ \
-I octoxxit/deps/boostorg/tokenizer/include/ \
-I octoxxit/deps/boostorg/tti/include/ \
-I octoxxit/deps/boostorg/tuple/include/ \
-I octoxxit/deps/boostorg/type_index/include/ \
-I octoxxit/deps/boostorg/type_traits/include/ \
-I octoxxit/deps/boostorg/typeof/include/ \
-I octoxxit/deps/boostorg/utility/include/ \
-I octoxxit/deps/boostorg/variant/include/ \
-I octoxxit/deps/boostorg/winapi/include/ \
-I octoxxit/deps/nlohmann/json/include/ \
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
find_package(octoxxit REQUIRED)
add_executable(app your_code.cpp)
target_link_libraries(app nxxm::octoxxit)
```

## If you don't use CMake : 
Add to your include path : 
```
INSTALL_PREFIX/include/octoxxit/deps/
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
