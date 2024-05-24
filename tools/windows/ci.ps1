param($arch, $tag)
$root_dir = Resolve-Path (Join-Path $PSScriptRoot "../../")

cd $root_dir
try
{
    cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S"$root_dir" -B"$root_dir/build" -G Ninja
    cmake --build "$root_dir/build" --config Release --target all --
    mkdir "$root_dir/tmp/build"
    Write-Host "$root_dir/build"
    Get-ChildItem -Path "$root_dir/build/src" -Filter "*.dll" -Recurse | ForEach-Object {
        try{
            # 对每个文件执行操作，例如输出文件名
            $Name = $_.BaseName
            $FullName = $_.FullName
            Write-Host $FullName
            mv "$FullName" "$root_dir/tmp/build/yukihana-$Name-win32-$arch-$tag.dll"
        }catch{
            Write-Host $_
        }
    }
}catch{
    exit 1
}