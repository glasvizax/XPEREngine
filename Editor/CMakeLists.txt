cmake_minimum_required(VERSION 3.10)
include(CSharpUtilities)

project(XPEREditor VERSION 0.1.0 LANGUAGES CSharp)

# getting engine src files
file(GLOB_RECURSE EDITOR_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cs" 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.resx" 
)
add_executable(Editor ${EDITOR_SRC})

set_property(SOURCE App.xaml PROPERTY VS_XAML_TYPE "ApplicationDefinition")

SET_PROPERTY(TARGET Editor PROPERTY VS_PACKAGE_REFERENCES "Serilog_2.9.0;Serilog.Sinks.Console_3.1.1")

target_compile_options(Editor PRIVATE "/langversion:latest")

set_target_properties(Editor PROPERTIES
DOTNET_SDK "Microsoft.NET.Sdk"
    DOTNET_TARGET_FRAMEWORK "net8.0-windows"
    WIN32_EXECUTABLE TRUE
    VS_GLOBAL_UseWindowsForms "true"
    VS_GLOBAL_ImplicitUsings "enable"
)