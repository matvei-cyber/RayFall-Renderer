**Deep debug** g++ src/main.cpp raylib/libraylib.a -o "app/RayFall Renderer.exe" -std=c++26 -O1 -I raylib -lopengl32 -lgdi32 -lwinmm -static -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2 -D_GLIBCXX_DEBUG

**DEBUG:** g++ src/main.cpp raylib/libraylib.a -o "app/RayFall Renderer.exe" -std=c++26 -O2 -I raylib -lopengl32 -lgdi32 -lwinmm -static -g -D_GLIBCXX_DEBUG

**RELEASE:** g++ src/main.cpp raylib/libraylib.a -o "app/RayFall Renderer.exe" -std=c++26 -O3 -flto -fno-math-errno -fno-trapping-math -ffp-contract=fast -I raylib -lopengl32 -lgdi32 -lwinmm -mwindows -static -DNDEBUG -s