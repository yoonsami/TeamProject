for /r %%f in (*.tga) do (
  .\texconv.exe -y -f R16G16B16A16_UNORM -srgb -ft dds "%%~f" -o "%%~dpf
)