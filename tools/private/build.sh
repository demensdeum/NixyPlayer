#!/bin/bash

human_readable() {
    local bytes=$1
    local kib=$(( 1024 ))
    local mib=$(( kib * 1024 ))
    local gib=$(( mib * 1024 ))
    local tib=$(( gib * 1024 ))

    if (( bytes >= tib )); then
        echo "$(awk "BEGIN {printf \"%.2f TiB\", $bytes / $tib}")"
    elif (( bytes >= gib )); then
        echo "$(awk "BEGIN {printf \"%.2f GiB\", $bytes / $gib}")"
    elif (( bytes >= mib )); then
        echo "$(awk "BEGIN {printf \"%.2f MiB\", $bytes / $mib}")"
    elif (( bytes >= kib )); then
        echo "$(awk "BEGIN {printf \"%.2f KiB\", $bytes / $kib}")"
    else
        echo "${bytes} B"
    fi
}

get_file_size() {
    local file_path="$1"

    if [ -f "$file_path" ]; then
        local file_size=$(stat -c %s "$file_path")
        echo "File Size: $(human_readable $file_size)"
    else
        echo "File not found: $file_path"
    fi
}

mkdir -p build
cd build

debug_build=false

if [[ "$*" == *"--debug-build"* ]]; then
    debug_build=true
fi

if [ "$debug_build" = true ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug ../src
else
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3" ../src
fi

start_time=$(date +%s%N)

make -j$(nproc)

if [ $? -eq 0 ]; then
    end_time=$(date +%s%N)
    duration=$(( (end_time - start_time) / 1000000 ))  # Duration in milliseconds

    end_time_seconds=$(( end_time / 1000000000 ))
    end_time_nanos=$(( end_time % 1000000000 ))

    end_time_formatted=$(date -d "@${end_time_seconds}" +"%Y-%m-%d %H:%M:%S")
    end_time_with_ms="${end_time_formatted}.$(( end_time_nanos / 1000000 ))"

    duration_seconds=$(( duration / 1000 ))
    duration_ms=$(( duration % 1000 ))

    duration_formatted="$(printf '%02d:%02d:%02d.%03d' \
                        $((duration_seconds / 3600)) \
                        $(( (duration_seconds % 3600) / 60 )) \
                        $((duration_seconds % 60 )) \
                        $duration_ms)"

    echo "Build completed successfully."
    echo "Build duration: ${duration_formatted}"

    get_file_size "./NixyPlayer"

    exit 0
else
    echo "Build failed."
    exit 1
fi
