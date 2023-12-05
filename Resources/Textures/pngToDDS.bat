for /r %%f in (*.png) do (
  .\texconv.exe -f R16G16B16A16_UNORM -srgb -ft dds "%%~f" -o "%%~dpf
)