/*
File:   build_script.c
Author: Taylor Robbins
Date:   07\04\2026
Description: 
	** A PigBuild-based build script for Box3D (https://github.com/erincatto/box3d)
*/

#include "pig_build.h"

#define DEBUG_BUILD                  1
#define BUILD_BOX3D_DLL              1
#define BUILD_SAMPLES                1
#define ONLY_BUILD_NON_EXISTANT_OBJS 0

#define BOX3D_SOURCE_FILES {          \
	"[ROOT]/src/aabb.c",              \
	"[ROOT]/src/aabb.h",              \
	"[ROOT]/src/algorithm.h",         \
	"[ROOT]/src/arena_allocator.c",   \
	"[ROOT]/src/arena_allocator.h",   \
	"[ROOT]/src/bitset.c",            \
	"[ROOT]/src/bitset.h",            \
	"[ROOT]/src/block_allocator.c",   \
	"[ROOT]/src/block_allocator.h",   \
	"[ROOT]/src/body.c",              \
	"[ROOT]/src/body.h",              \
	"[ROOT]/src/broad_phase.c",       \
	"[ROOT]/src/broad_phase.h",       \
	"[ROOT]/src/capsule.c",           \
	"[ROOT]/src/compound.c",          \
	"[ROOT]/src/compound.h",          \
	"[ROOT]/src/constraint_graph.c",  \
	"[ROOT]/src/constraint_graph.h",  \
	"[ROOT]/src/contact.c",           \
	"[ROOT]/src/contact.h",           \
	"[ROOT]/src/contact_solver.c",    \
	"[ROOT]/src/contact_solver.h",    \
	"[ROOT]/src/container.h",         \
	"[ROOT]/src/convex_manifold.c",   \
	"[ROOT]/src/core.c",              \
	"[ROOT]/src/core.h",              \
	"[ROOT]/src/ctz.h",               \
	"[ROOT]/src/distance.c",          \
	"[ROOT]/src/distance_joint.c",    \
	"[ROOT]/src/dynamic_tree.c",      \
	"[ROOT]/src/height_field.c",      \
	"[ROOT]/src/hull.c",              \
	"[ROOT]/src/id_pool.c",           \
	"[ROOT]/src/id_pool.h",           \
	"[ROOT]/src/island.c",            \
	"[ROOT]/src/island.h",            \
	"[ROOT]/src/joint.c",             \
	"[ROOT]/src/joint.h",             \
	"[ROOT]/src/manifold.c",          \
	"[ROOT]/src/manifold.h",          \
	"[ROOT]/src/math_functions.c",    \
	"[ROOT]/src/math_internal.h",     \
	"[ROOT]/src/mesh.c",              \
	"[ROOT]/src/mesh_contact.c",      \
	"[ROOT]/src/motor_joint.c",       \
	"[ROOT]/src/mover.c",             \
	"[ROOT]/src/parallel_for.c",      \
	"[ROOT]/src/parallel_for.h",      \
	"[ROOT]/src/parallel_joint.c",    \
	"[ROOT]/src/physics_world.c",     \
	"[ROOT]/src/physics_world.h",     \
	"[ROOT]/src/platform.h",          \
	"[ROOT]/src/prismatic_joint.c",   \
	"[ROOT]/src/qsort.h",             \
	"[ROOT]/src/recording.c",         \
	"[ROOT]/src/recording.h",         \
	"[ROOT]/src/recording_ops.inl",   \
	"[ROOT]/src/recording_replay.c",  \
	"[ROOT]/src/recording_replay.h",  \
	"[ROOT]/src/world_snapshot.c",    \
	"[ROOT]/src/world_snapshot.h",    \
	"[ROOT]/src/revolute_joint.c",    \
	"[ROOT]/src/scheduler.c",         \
	"[ROOT]/src/scheduler.h",         \
	"[ROOT]/src/sensor.c",            \
	"[ROOT]/src/sensor.h",            \
	"[ROOT]/src/shape.c",             \
	"[ROOT]/src/shape.h",             \
	"[ROOT]/src/simd.c",              \
	"[ROOT]/src/simd.h",              \
	"[ROOT]/src/solver.c",            \
	"[ROOT]/src/solver.h",            \
	"[ROOT]/src/solver_set.c",        \
	"[ROOT]/src/solver_set.h",        \
	"[ROOT]/src/sphere.c",            \
	"[ROOT]/src/spherical_joint.c",   \
	"[ROOT]/src/table.c",             \
	"[ROOT]/src/table.h",             \
	"[ROOT]/src/timer.c",             \
	"[ROOT]/src/triangle_manifold.c", \
	"[ROOT]/src/types.c",             \
	"[ROOT]/src/verstable.h",         \
	"[ROOT]/src/weld_joint.c",        \
	"[ROOT]/src/wheel_joint.c",       \
}

#define BOX3D_INCLUDE_FILES {                \
	"[ROOT]/include/box3d/base.h",           \
	"[ROOT]/include/box3d/box3d.h",          \
	"[ROOT]/include/box3d/collision.h",      \
	"[ROOT]/include/box3d/config.h",         \
	"[ROOT]/include/box3d/constants.h",      \
	"[ROOT]/include/box3d/id.h",             \
	"[ROOT]/include/box3d/math_functions.h", \
	"[ROOT]/include/box3d/types.h",          \
}

#define BOX3D_SHARED_FILES {               \
	"[ROOT]/shared/benchmarks.c",          \
	"[ROOT]/shared/benchmarks.h",          \
	"[ROOT]/shared/determinism.c",         \
	"[ROOT]/shared/determinism.h",         \
	"[ROOT]/shared/human.c",               \
	"[ROOT]/shared/human.h",               \
	"[ROOT]/shared/overflow_color.c",      \
	"[ROOT]/shared/overflow_color.h",      \
	"[ROOT]/shared/stability.c",           \
	"[ROOT]/shared/stability.h",           \
	"[ROOT]/shared/utils.c",               \
	"[ROOT]/shared/utils.h",               \
}

#if BUILDING_ON_WINDOWS
#define NFD_SOURCE_PATH "nfd/src/nfd_win.cpp"
#elif BUILDING_ON_LINUX
// #define NFD_SOURCE_PATH "nfd_gtk.cpp"
#define NFD_SOURCE_PATH "nfd/src/nfd_portal.cpp"
#elif BUILDING_ON_OSX
#define NFD_SOURCE_PATH "nfd/src/nfd_cocoa.m"
#endif

#define SAMPLE_FILES {                        \
	"[ROOT]/samples/main.cpp",                \
	/*Host*/                                  \
	"[ROOT]/samples/host/camera.cpp",         \
	"[ROOT]/samples/host/camera.h",           \
	"[ROOT]/samples/host/gui.cpp",            \
	"[ROOT]/samples/host/gui.h",              \
	"[ROOT]/samples/host/imgui_impl.cpp",     \
	"[ROOT]/samples/host/sokol_app_impl.c",   \
	"[ROOT]/samples/sample.cpp",              \
	"[ROOT]/samples/sample.h",                \
	"[ROOT]/samples/mesh_loader.cpp",         \
	"[ROOT]/samples/mesh_loader.h",           \
	"[ROOT]/samples/jsmn.h",                  \
	"[ROOT]/samples/tiny_obj_loader.h",       \
	"[ROOT]/samples/earcut.h",                \
	/*GFX_FILES*/                             \
	"[ROOT]/samples/gfx/arena.c",             \
	"[ROOT]/samples/gfx/arena.h",             \
	"[ROOT]/samples/gfx/debug_adapter.c",     \
	"[ROOT]/samples/gfx/debug_adapter.h",     \
	"[ROOT]/samples/gfx/debug_shapes.c",      \
	"[ROOT]/samples/gfx/debug_shapes.h",      \
	"[ROOT]/samples/gfx/draw_overlay.h",      \
	"[ROOT]/samples/gfx/draw.c",              \
	"[ROOT]/samples/gfx/draw.h",              \
	"[ROOT]/samples/gfx/edges.c",             \
	"[ROOT]/samples/gfx/edges.h",             \
	"[ROOT]/samples/gfx/geometry_registry.c", \
	"[ROOT]/samples/gfx/geometry_registry.h", \
	"[ROOT]/samples/gfx/gtao.c",              \
	"[ROOT]/samples/gfx/gtao.h",              \
	"[ROOT]/samples/gfx/highlight_mask.c",    \
	"[ROOT]/samples/gfx/highlight_mask.h",    \
	"[ROOT]/samples/gfx/highlight_outline.c", \
	"[ROOT]/samples/gfx/highlight_outline.h", \
	"[ROOT]/samples/gfx/highlight_target.c",  \
	"[ROOT]/samples/gfx/highlight_target.h",  \
	"[ROOT]/samples/gfx/ibl.c",               \
	"[ROOT]/samples/gfx/ibl.h",               \
	"[ROOT]/samples/gfx/overlay.c",           \
	"[ROOT]/samples/gfx/overlay.h",           \
	"[ROOT]/samples/gfx/projection.h",        \
	"[ROOT]/samples/gfx/qsort.h",             \
	"[ROOT]/samples/gfx/renderer.c",          \
	"[ROOT]/samples/gfx/renderer.h",          \
	"[ROOT]/samples/gfx/scene_target.c",      \
	"[ROOT]/samples/gfx/scene_target.h",      \
	"[ROOT]/samples/gfx/shadow.c",            \
	"[ROOT]/samples/gfx/shadow.h",            \
	"[ROOT]/samples/gfx/sky.c",               \
	"[ROOT]/samples/gfx/sky.h",               \
	"[ROOT]/samples/gfx/sokol_impl.c",        \
	"[ROOT]/samples/gfx/text.c",              \
	"[ROOT]/samples/gfx/text.h",              \
	"[ROOT]/samples/gfx/tone_map.c",          \
	"[ROOT]/samples/gfx/tone_map.h",          \
	"[ROOT]/samples/gfx/utility.h",           \
	/*Samples*/                               \
	"[ROOT]/samples/sample_benchmark.cpp",    \
	"[ROOT]/samples/sample_bodies.cpp",       \
	"[ROOT]/samples/sample_character.cpp",    \
	"[ROOT]/samples/sample_collision.cpp",    \
	"[ROOT]/samples/sample_compound.cpp",     \
	"[ROOT]/samples/sample_continuous.cpp",   \
	"[ROOT]/samples/sample_determinism.cpp",  \
	"[ROOT]/samples/sample_events.cpp",       \
	"[ROOT]/samples/sample_geometry.cpp",     \
	"[ROOT]/samples/sample_issues.cpp",       \
	"[ROOT]/samples/sample_joint.cpp",        \
	"[ROOT]/samples/sample_manifold.cpp",     \
	"[ROOT]/samples/sample_mesh.cpp",         \
	"[ROOT]/samples/sample_ragdoll.cpp",      \
	"[ROOT]/samples/sample_replay.cpp",       \
	"[ROOT]/samples/sample_robustness.cpp",   \
	"[ROOT]/samples/sample_shapes.cpp",       \
	"[ROOT]/samples/sample_stacking.cpp",     \
	"[ROOT]/samples/sample_tree.cpp",         \
	"[ROOT]/samples/sample_world.cpp",        \
	/*Dear Imgui*/                            \
    "imgui/imgui.cpp",                        \
    "imgui/imgui_draw.cpp",                   \
    "imgui/imgui_tables.cpp",                 \
    "imgui/imgui_widgets.cpp",                \
    "imgui/imgui_demo.cpp",                   \
    /*ImPlot*/                                \
    "implot/implot.cpp",                      \
    "implot/implot_items.cpp",                \
    "implot/implot_demo.cpp",                 \
    "implot/implot.h",                        \
    "implot/implot_internal.h",               \
    NFD_SOURCE_PATH,                          \
}

#if DEBUG_BUILD
#define IF_DEBUG(...)   __VA_ARGS__
#define IF_RELEASE(...) //nothing
#else
#define IF_DEBUG(...)   //nothing
#define IF_RELEASE(...) __VA_ARGS__
#endif
#if BUILDING_ON_WINDOWS
#define IF_WINDOWS(...)     __VA_ARGS__
#define IF_NOT_WINDOWS(...) //nothing
#else
#define IF_WINDOWS(...)     //nothing
#define IF_NOT_WINDOWS(...) __VA_ARGS__
#endif
#if BUILDING_ON_LINUX
#define IF_LINUX(...)     __VA_ARGS__
#define IF_NOT_LINUX(...) //nothing
#else
#define IF_LINUX(...)     //nothing
#define IF_NOT_LINUX(...) __VA_ARGS__
#endif
#if BUILDING_ON_OSX
#define IF_OSX(...)     __VA_ARGS__
#define IF_NOT_OSX(...) //nothing
#else
#define IF_OSX(...)     //nothing
#define IF_NOT_OSX(...) __VA_ARGS__
#endif

#define T_SAMPLES   "|Samples"
#define T_BOX3D_DLL "|Box3Ddll"

void DownloadDearImgui();
void DownloadImplot();
void DownloadNativeFileDialog();

bool IsCompilableExt(Str pathStr)
{
	Str extension = GetFileExtPart(pathStr, false);
	return (
		StrAnyCaseEquals(extension, StrLit(".c")) ||
		StrAnyCaseEquals(extension, StrLit(".cpp")) ||
		StrAnyCaseEquals(extension, StrLit(".m")) ||
		StrAnyCaseEquals(extension, StrLit(".mm"))
	);
}
bool IsCppExt(Str pathStr)
{
	Str extension = GetFileExtPart(pathStr, false);
	return (
		StrAnyCaseEquals(extension, StrLit(".cpp")) ||
		StrAnyCaseEquals(extension, StrLit(".mm"))
	);
}
bool IsObjectiveCExt(Str pathStr)
{
	Str extension = GetFileExtPart(pathStr, false);
	return (
		StrAnyCaseEquals(extension, StrLit(".m")) ||
		StrAnyCaseEquals(extension, StrLit(".mm"))
	);
}

void BuildObjectIfNeeded(Str sourcePath, Str objectPath, CliArgs* compilerFlags, StrArray* commonTags, const char* extraTag)
{
	if (!ONLY_BUILD_NON_EXISTANT_OBJS || !DoesFileExist(objectPath))
	{
		Str sourceFileName = GetFileNamePart(sourcePath, true);
		PrintLine("[Compiling... %.*s -> %.*s]", StrPrint(sourceFileName), StrPrint(objectPath));
		
		bool isCpp = IsCppExt(sourceFileName);
		bool isObjectiveC = IsObjectiveCExt(sourceFileName);
		if (StrAnyCaseEquals(sourceFileName, StrLit("sokol_app_impl.c"))) { isObjectiveC = true; }
		if (StrAnyCaseEquals(sourceFileName, StrLit("sokol_impl.c"))) { isObjectiveC = true; }
		
		CliArgs args = EMPTY;
		AddTaggedArg(&args, T_MSVC_CL, CL_COMPILE);
		AddTaggedArg(&args, T_CLANG,   CLANG_COMPILE);
		AddTaggedArgStr(&args, T_MSVC_CL, CL_OBJ_FILE, objectPath);
		AddTaggedArgStr(&args, T_CLANG,   CLANG_OUTPUT_FILE, objectPath);
		AddArgList(&args, compilerFlags);
		AddArgStr(&args, CLI_QUOTED_ARG, sourcePath);
		
		StrArray tags = EMPTY;
		AddTag(&tags, T_OBJECT);
		if (extraTag != nullptr) { AddTag(&tags, extraTag); }
		AddTag(&tags, isObjectiveC ? (isCpp ? T_LANG_OBJECTIVECPP : T_LANG_OBJECTIVEC) : (isCpp ? T_LANG_CPP : T_LANG_C));
		AddStrArray(&tags, commonTags);
		IF_WINDOWS(AddTag(&tags, T_MSVC_CL));
		IF_WINDOWS(AddTag(&tags, T_MSVC_CL_OR_LINK));
		IF_NOT_WINDOWS(AddTag(&tags, T_CLANG));
		if (StrAnyCaseStartsWith(sourceFileName, StrLit("nfd_"))) { AddTag(&tags, "|nfd"); }
		
		Str compilerExe = BUILDING_ON_WINDOWS ? StrLit(EXE_MSVC_CL) : StrLit(EXE_CLANG);
		RunCliProgramAndExitOnFailureTags(compilerExe, tags, &args, FormatStr("Failed to compile \"%.*s\"", StrPrint(sourceFileName)));
		AssertFileExist(objectPath, true);
	}
}

int main(int argc, char* argv[])
{
	RecompileIfNeeded(StrArray_Empty);
	#if BUILDING_ON_WINDOWS
	bool isMsvcInitialized = WasMsvcDevBatchRun();
	#endif
	const char* sharedPathsNt[] = BOX3D_SHARED_FILES;
	const char* dllSourcePathsNt[] = BOX3D_SOURCE_FILES;
	const char* includePathsNt[] = BOX3D_INCLUDE_FILES;
	const char* samplesSourcePathsNt[] = SAMPLE_FILES;
	Str box3dDllPath = StrLit("box3d" DLL_EXT);
	Str box3dLibPath = StrLit("box3d" LIB_EXT);
	Str samplesExePath = StrLit("box3d_samples" EXE_EXT);
	IF_WINDOWS(Str box3dPdbPath = StrLit("box3d.pdb"));
	
	DownloadDearImgui();
	DownloadImplot();
	DownloadNativeFileDialog();
	
	CliArgs compilerFlags = EMPTY;
	CliArgs linkerFlags = EMPTY;
	AddTaggedArg(&compilerFlags,    T_MSVC_CL, CL_FULL_FILE_PATHS);
	AddTaggedArg(&compilerFlags,    T_MSVC_CL, CL_NO_LOGO);
	AddTaggedArg(&linkerFlags,      T_MSVC_CL, LINK_DISABLE_INCREMENTAL);
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL, CL_CONFIGURE_EXCEPTION_HANDLING, "s"); //enable stack-unwinding
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL, CL_CONFIGURE_EXCEPTION_HANDLING, "c"); //extern "C" functions don't through exceptions
	AddTaggedArg(&compilerFlags,    T_CLANG,   CLANG_FULL_FILE_PATHS);
	
	AddTaggedArgNt(&compilerFlags,  T_CLANG,   CLANG_INCLUDE_DIR, "[ROOT]/include");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL, CL_INCLUDE_DIR,    "[ROOT]/include");
	AddTaggedArgNt(&compilerFlags,  T_CLANG,   CLANG_INCLUDE_DIR, "[ROOT]/shared");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL, CL_INCLUDE_DIR,    "[ROOT]/shared");
	AddTaggedArgNt(&compilerFlags,  T_CLANG,   CLANG_INCLUDE_DIR, "[ROOT]/bin");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL, CL_INCLUDE_DIR,    "[ROOT]/bin");
	AddTaggedArgNt(&compilerFlags,  T_CLANG,   CLANG_INCLUDE_DIR, "[ROOT]/data");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL, CL_INCLUDE_DIR,    "[ROOT]/data");
	
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_SAMPLES, CLANG_INCLUDE_DIR, "[ROOT]/samples");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_SAMPLES, CL_INCLUDE_DIR,    "[ROOT]/samples");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_SAMPLES, CLANG_INCLUDE_DIR, "[ROOT]/samples/shaders/generated");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_SAMPLES, CL_INCLUDE_DIR,    "[ROOT]/samples/shaders/generated");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_SAMPLES, CLANG_INCLUDE_DIR, "[ROOT]/extern/sokol");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_SAMPLES, CL_INCLUDE_DIR,    "[ROOT]/extern/sokol");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_SAMPLES, CLANG_INCLUDE_DIR, "imgui");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_SAMPLES, CL_INCLUDE_DIR,    "imgui");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_SAMPLES, CLANG_INCLUDE_DIR, "implot");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_SAMPLES, CL_INCLUDE_DIR,    "implot");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_SAMPLES, CLANG_INCLUDE_DIR, "nfd/src/include");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_SAMPLES, CL_INCLUDE_DIR,    "nfd/src/include");
	
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_WINDOWS,   CL_DEFINE,    "SOKOL_D3D11");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_LINUX,     CLANG_DEFINE, "SOKOL_GLCORE");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_OSX,       CLANG_DEFINE, "SOKOL_METAL");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_OSX,       CLANG_DEFINE, "NFD_MACOS_ALLOWEDCONTENTTYPES=1");
	
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_BOX3D_DLL, CL_DEFINE,    "box3d_EXPORTS");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_BOX3D_DLL, CLANG_DEFINE, "box3d_EXPORTS");
	
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_LANG_C,            CL_LANG_VERSION, "c17");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_LANG_C,            CLANG_LANG_VERSION, "gnu17");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_LANG_OBJECTIVEC,   CLANG_LANG_VERSION, "gnu17");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_LANG_CPP,          CL_LANG_VERSION, "c++20");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_LANG_CPP,          CLANG_LANG_VERSION, "c++20");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_LANG_CPP,          CLANG_SYSTEM_LIBRARY, "stdc++");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_LANG_OBJECTIVECPP, CLANG_SYSTEM_LIBRARY, "stdc++");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_LANG_OBJECTIVEC,   CLANG_LANGUAGE, "objective-c");
	AddTaggedArgNt(&compilerFlags,  T_CLANG   T_LANG_OBJECTIVECPP, CLANG_LANGUAGE, "objective-c++");
	AddTaggedArg(&compilerFlags,    T_CLANG   T_LANG_OBJECTIVEC   "|nfd==false",   CLANG_ENABLE_OBJC_ARC);
	AddTaggedArg(&compilerFlags,    T_CLANG   T_LANG_OBJECTIVECPP "|nfd==false", CLANG_ENABLE_OBJC_ARC);
	
	AddTaggedArg(&compilerFlags,    T_MSVC_CL T_DEBUG_BUILD,        CL_DEBUG_INFO);
	AddTaggedArg(&compilerFlags,    T_MSVC_CL T_DEBUG_BUILD,        CL_STD_LIB_DYNAMIC_DBG);
	AddTaggedArg(&compilerFlags,    T_MSVC_CL T_RELEASE_BUILD,      CL_STD_LIB_DYNAMIC);
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_DEBUG_BUILD,        CL_OPTIMIZATION_LEVEL, "d");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_RELEASE_BUILD,      CL_OPTIMIZATION_LEVEL, "2");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_RELEASE_BUILD,      CL_OPTIMIZATION_LEVEL, "y");
	AddTaggedArgNt(&compilerFlags,  T_MSVC_CL T_RELEASE_BUILD,      CL_OPTIMIZATION_LEVEL, "t");
	AddTaggedArgNt(&compilerFlags,  T_CLANG T_UNIX T_DEBUG_BUILD,   CLANG_OPTIMIZATION_LEVEL, "0");
	AddTaggedArgNt(&compilerFlags,  T_CLANG T_UNIX T_RELEASE_BUILD, CLANG_OPTIMIZATION_LEVEL, "2");
	AddTaggedArgNt(&compilerFlags,  T_CLANG T_UNIX T_DEBUG_BUILD,   CLANG_DEBUG_INFO, "dwarf-4");
	
	// AddTaggedArgNt(&compilerFlags,  T_MSVC_CL, CL_WARNING_LEVEL, "X");
	// AddTaggedArgNt(&compilerFlags,  T_MSVC_CL, CL_WARNING_LEVEL, "4");
	// AddTaggedArgNt(&compilerFlags,  T_CLANG,   CLANG_WARNING_LEVEL, "all");
	// AddTaggedArgNt(&compilerFlags,  T_CLANG,   CLANG_WARNING_LEVEL, "extra");
	AddTaggedArgNt(&compilerFlags,  T_CLANG,   CLANG_DISABLE_WARNING, "unused-command-line-argument"); //warning: -Z-reserved-lib-stdc++: 'linker' input unused
	AddTaggedArgNt(&compilerFlags,  T_CLANG,   CLANG_DISABLE_WARNING, "deprecated-enum-enum-conversion"); //warning: bitwise operation between different enumeration types ('ImGuiButtonFlags_' and 'ImGuiButtonFlagsPrivate_') is deprecated
	
	AddTaggedArgNt(&linkerFlags,  T_MSVC_LINK T_WINDOWS T_SAMPLES,  CLI_QUOTED_ARG, "Ole32.lib"); //Required for things like CoInitializeEx which nfd depends on
	
	// AddTaggedArgNt(&linkerFlags, T_CLANG T_OSX T_SAMPLES, CLANG_FRAMEWORK, "CoreText"); //For functions like CTFontCollectionCreateMatchingFontDescriptors in os_font.h OsReadPlatformFont
	AddTaggedArgNt(&linkerFlags, T_CLANG T_OSX T_SAMPLES, CLANG_FRAMEWORK, "Foundation");
	AddTaggedArgNt(&linkerFlags, T_CLANG T_OSX T_SAMPLES, CLANG_FRAMEWORK, "Cocoa");
	AddTaggedArgNt(&linkerFlags, T_CLANG T_OSX T_SAMPLES, CLANG_FRAMEWORK, "QuartzCore");
	// AddTaggedArgNt(&linkerFlags, T_CLANG T_OSX T_SAMPLES, CLANG_FRAMEWORK, "CoreFoundation");
	AddTaggedArgNt(&linkerFlags, T_CLANG T_OSX T_SAMPLES, CLANG_FRAMEWORK, "Metal");
	AddTaggedArgNt(&linkerFlags, T_CLANG T_OSX T_SAMPLES, CLANG_FRAMEWORK, "MetalKit");
	AddTaggedArgNt(&linkerFlags, T_CLANG T_OSX T_SAMPLES, CLANG_FRAMEWORK, "AppKit"); //required for nfd
	AddTaggedArgNt(&linkerFlags, T_CLANG T_OSX T_SAMPLES, CLANG_FRAMEWORK, "UniformTypeIdentifiers");
	
	StrArray commonTags = EMPTY;
	IF_DEBUG(AddTag(&commonTags, "DEBUG_BUILD"));
	IF_WINDOWS(AddTag(&commonTags, T_WINDOWS));
	IF_LINUX(AddTag(&commonTags, T_LINUX));
	IF_OSX(AddTag(&commonTags, T_OSX));
	IF_NOT_WINDOWS(AddTag(&commonTags, T_UNIX));
	
	// +==============================+
	// |    Compile Shared Objects    |
	// +==============================+
	#if (BUILD_BOX3D_DLL || BUILD_SAMPLES)
	StrArray sharedObjectPaths = EMPTY;
	for (u64 fIndex = 0; fIndex < ArrayCount(sharedPathsNt); fIndex++)
	{
		Str sourcePath = MakeStrNt(sharedPathsNt[fIndex]);
		if (IsCompilableExt(sourcePath))
		{
			Str objectPath = ChangePathExtension(GetFileNamePart(sourcePath, true), StrLit(OBJ_EXT), true);
			BuildObjectIfNeeded(sourcePath, objectPath, &compilerFlags, &commonTags, nullptr);
			AddStr(&sharedObjectPaths, objectPath);
		}
	}
	#endif //(BUILD_BOX3D_DLL || BUILD_SAMPLES)
	
	// +==============================+
	// |     Compile DLL Objects      |
	// +==============================+
	#if BUILD_BOX3D_DLL
	StrArray dllObjectPaths = EMPTY;
	for (u64 fIndex = 0; fIndex < ArrayCount(dllSourcePathsNt); fIndex++)
	{
		Str sourcePath = MakeStrNt(dllSourcePathsNt[fIndex]);
		if (IsCompilableExt(sourcePath))
		{
			Str objectPath = ChangePathExtension(GetFileNamePart(sourcePath, true), StrLit(OBJ_EXT), true);
			BuildObjectIfNeeded(sourcePath, objectPath, &compilerFlags, &commonTags, T_BOX3D_DLL);
			AddStr(&dllObjectPaths, objectPath);
		}
	}
	#endif //BUILD_BOX3D_DLL
	
	// +==============================+
	// |    Compile Sample Objects    |
	// +==============================+
	#if BUILD_SAMPLES
	StrArray samplesObjectPaths = EMPTY;
	for (u64 fIndex = 0; fIndex < ArrayCount(samplesSourcePathsNt); fIndex++)
	{
		Str sourcePath = MakeStrNt(samplesSourcePathsNt[fIndex]);
		if (IsCompilableExt(sourcePath))
		{
			Str objectPath = ChangePathExtension(GetFileNamePart(sourcePath, true), StrLit(OBJ_EXT), true);
			BuildObjectIfNeeded(sourcePath, objectPath, &compilerFlags, &commonTags, T_SAMPLES);
			AddStr(&samplesObjectPaths, objectPath);
		}
	}
	#endif //BUILD_SAMPLES
	
	// +==============================+
	// |        Link box3d.dll        |
	// +==============================+
	#if BUILD_BOX3D_DLL
	{
		PrintLine("[Linking %.*s...]", StrPrint(box3dDllPath));
		
		CliArgs args = EMPTY;
		AddTaggedArg(&args,    T_MSVC_LINK, LINK_BUILD_DLL);
		AddTaggedArg(&args,    T_CLANG,     CLANG_BUILD_SHARED_LIB);
		AddTaggedArg(&args,    T_CLANG,     CLANG_fPIC);
		AddTaggedArgStr(&args, T_MSVC_LINK, LINK_OUTPUT_FILE, box3dDllPath);
		AddTaggedArgStr(&args, T_CLANG,     CLANG_OUTPUT_FILE, box3dDllPath);
		AddArgList(&args, &compilerFlags);
		AddArgList(&args, &linkerFlags);
		for (u64 oIndex = 0; oIndex < sharedObjectPaths.length; oIndex++) { AddArgStr(&args, CLI_QUOTED_ARG, sharedObjectPaths.strings[oIndex]); }
		for (u64 oIndex = 0; oIndex < dllObjectPaths.length; oIndex++) { AddArgStr(&args, CLI_QUOTED_ARG, dllObjectPaths.strings[oIndex]); }
		
		StrArray tags = EMPTY;
		AddTag(&tags, T_LIBRARY);
		AddTag(&tags, T_BOX3D_DLL);
		AddStrArray(&tags, &commonTags);
		IF_WINDOWS(AddTag(&tags, T_MSVC_LINK));
		IF_WINDOWS(AddTag(&tags, T_MSVC_CL_OR_LINK));
		IF_NOT_WINDOWS(AddTag(&tags, T_CLANG));
		
		Str linkerExe = BUILDING_ON_WINDOWS ? StrLit(EXE_MSVC_LINK) : StrLit(EXE_CLANG);
		RunCliProgramAndExitOnFailureTags(linkerExe, tags, &args, FormatStr("Failed to link \"%.*s\"", StrPrint(box3dDllPath)));
		AssertFileExist(box3dDllPath, true);
		PrintLine("[Done Linking %.*s!]", StrPrint(box3dDllPath));
	}
	#endif
	
	// +==============================+
	// |       Link Samples.exe       |
	// +==============================+
	#if BUILD_SAMPLES
	{
		PrintLine("[Linking %.*s...]", StrPrint(samplesExePath));
		
		CliArgs args = EMPTY;
		// AddTaggedArg(&args,    T_MSVC_LINK, LINK_BUILD_DLL);
		// AddTaggedArg(&args,    T_CLANG,     CLANG_BUILD_SHARED_LIB);
		// AddTaggedArg(&args,    T_CLANG,     CLANG_fPIC);
		AddTaggedArgStr(&args, T_MSVC_LINK, LINK_OUTPUT_FILE, samplesExePath);
		AddTaggedArgStr(&args, T_CLANG,     CLANG_OUTPUT_FILE, samplesExePath);
		AddArgList(&args, &compilerFlags);
		AddArgList(&args, &linkerFlags);
		for (u64 oIndex = 0; oIndex < sharedObjectPaths.length; oIndex++) { AddArgStr(&args, CLI_QUOTED_ARG, sharedObjectPaths.strings[oIndex]); }
		for (u64 oIndex = 0; oIndex < samplesObjectPaths.length; oIndex++) { AddArgStr(&args, CLI_QUOTED_ARG, samplesObjectPaths.strings[oIndex]); }
		AddArgStr(&args, CLI_QUOTED_ARG, box3dLibPath);
		
		StrArray tags = EMPTY;
		AddTag(&tags, T_LIBRARY);
		AddTag(&tags, T_SAMPLES);
		AddStrArray(&tags, &commonTags);
		IF_WINDOWS(AddTag(&tags, T_MSVC_LINK));
		IF_WINDOWS(AddTag(&tags, T_MSVC_CL_OR_LINK));
		IF_NOT_WINDOWS(AddTag(&tags, T_CLANG));
		
		Str linkerExe = BUILDING_ON_WINDOWS ? StrLit(EXE_MSVC_LINK) : StrLit(EXE_CLANG);
		RunCliProgramAndExitOnFailureTags(linkerExe, tags, &args, FormatStr("Failed to link \"%.*s\"", StrPrint(samplesExePath)));
		AssertFileExist(samplesExePath, true);
		PrintLine("[Done Linking %.*s!]", StrPrint(samplesExePath));
	}
	#endif
}

void DownloadDearImgui()
{
	Str imguiUrl = StrLit("https://github.com/ocornut/imgui/archive/refs/tags/v1.92.7.zip");
	Str imguiZipPath = StrLit("imgui_v1.92.7.zip");
	Str imguiZipRootFolder = StrLit("imgui-1.92.7");
	Str imguiFolderPath = StrLit("imgui");
	if (!DoesFileExist(imguiZipPath) || !DoesFolderExist(imguiFolderPath))
	{
		PrintLine("Downloading Dear ImGui from \"%.*s\"", StrPrint(imguiUrl));
		DownloadAndExtractArchive(
			imguiUrl,
			imguiZipPath,
			2247481, 0x223CB155060498F1,
			imguiFolderPath,
			imguiZipRootFolder
		);
	}
}

void DownloadImplot()
{
	Str implotUrl = StrLit("https://github.com/epezent/implot/archive/refs/tags/v0.17.zip");
	Str implotZipPath = StrLit("implot-v0.17.zip");
	Str implotZipRootFolder = StrLit("implot-0.17");
	Str implotFolderPath = StrLit("implot");
	if (!DoesFileExist(implotZipPath) || !DoesFolderExist(implotFolderPath))
	{
		PrintLine("Downloading ImPlot from \"%.*s\"", StrPrint(implotUrl));
		DownloadAndExtractArchive(
			implotUrl,
			implotZipPath,
			150841, 0xA40A5F3CACFB33C3,
			implotFolderPath,
			implotZipRootFolder
		);
	}
}

void DownloadNativeFileDialog()
{
	Str nfdUrl = StrLit("https://github.com/btzy/nativefiledialog-extended/archive/refs/tags/v1.3.0.zip");
	Str nfdZipPath = StrLit("nativefiledialog-extended-v1.3.0.zip");
	Str nfdZipRootFolder = StrLit("nativefiledialog-extended-1.3.0");
	Str nfdFolderPath = StrLit("nfd");
	if (!DoesFileExist(nfdZipPath) || !DoesFolderExist(nfdFolderPath))
	{
		PrintLine("Downloading NativeFileDialog-Extended from \"%.*s\"", StrPrint(nfdUrl));
		DownloadAndExtractArchive(
			nfdUrl,
			nfdZipPath,
			451462, 0x783D44A34B263389,
			nfdFolderPath,
			nfdZipRootFolder
		);
	}
}
