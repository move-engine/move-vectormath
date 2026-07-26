CPMAddPackage(
  NAME rtm
  GITHUB_REPOSITORY nfrechette/rtm
  GIT_TAG 745bd25673d93b46941eda55e0993327dbc12b53 # v2.3.1
  DOWNLOAD_ONLY YES
)

add_library(rtm INTERFACE)
target_include_directories(rtm INTERFACE ${rtm_SOURCE_DIR}/includes)
