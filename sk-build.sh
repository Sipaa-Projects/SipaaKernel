if [ ! -e "./sk-build/bin/Debug/net7.0/sk-build" ]; then
    dotnet build ./sk-build/sk-build.csproj
fi

if [ "$1" == "--build-sb" ]; then
    dotnet build ./sk-build/sk-build.csproj
    shift
    ./sk-build/bin/Debug/net7.0/sk-build $@
else
    ./sk-build/bin/Debug/net7.0/sk-build $@
fi
