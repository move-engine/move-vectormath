CPMAddPackage(
  NAME rtm
  GITHUB_REPOSITORY nfrechette/rtm
  #GIT_TAG v2.1.5
  GIT_TAG 26a4d04462c25a041d6aba3db7238e0209346b3d
  DOWNLOAD_ONLY YES
)

add_library(rtm INTERFACE)
target_include_directories(rtm INTERFACE ${rtm_SOURCE_DIR}/includes)