#
#
#


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := catcake

LOCAL_SRC_FILES := \
	catcake/source/cdt/ck_cdt_aabb.cpp \
	catcake/source/cdt/ck_cdt_box.cpp \
	catcake/source/cdt/ck_cdt_ray.cpp \
	catcake/source/cdt/ck_cdt_sph.cpp \
	catcake/source/cdt/ck_cdt_tri.cpp \
	catcake/source/conf/ck_conf.cpp \
	catcake/source/conf/ck_conf_mgr.cpp \
	catcake/source/conf/ck_conf_parse.cpp \
	catcake/source/conf/ck_ent.cpp \
	catcake/source/dbg/ck_dbg_mgr.cpp \
	catcake/source/dbg/ck_dbg_mgr_draw.cpp \
	catcake/source/dbg/ck_dbg_mgr_dump.cpp \
	catcake/source/dbg/ck_dbg_mgr_font.cpp \
	catcake/source/draw/ck_col.cpp \
	catcake/source/draw/ck_draw.cpp \
	catcake/source/draw/ck_draw_mgr.cpp \
	catcake/source/draw/ck_font.cpp \
	catcake/source/draw/ck_lit.cpp \
	catcake/source/draw/ck_lts.cpp \
	catcake/source/draw/ck_mdl.cpp \
	catcake/source/draw/ck_mdl_data.cpp \
	catcake/source/draw/ck_mot.cpp \
	catcake/source/draw/ck_mot_data.cpp \
	catcake/source/draw/ck_node.cpp \
	catcake/source/draw/ck_prim.cpp \
	catcake/source/draw/ck_rend.cpp \
	catcake/source/draw/ck_scr.cpp \
	catcake/source/draw/ck_shd.cpp \
	catcake/source/draw/ck_sprt.cpp \
	catcake/source/draw/ck_tex.cpp \
	catcake/source/draw/rend/ck_rend_light.cpp \
	catcake/source/draw/rend/ck_rend_shader.cpp \
	catcake/source/gen/ck_id.cpp \
	catcake/source/key/ck_key_mgr.cpp \
	catcake/source/low_level_api/android/ck_low_level_sys_android.cpp \
	catcake/source/low_level_api/ck_low_level_draw_opengl.cpp \
	catcake/source/low_level_api/ck_low_level_io_gen.cpp \
	catcake/source/low_level_api/ck_low_level_math_gen.cpp \
	catcake/source/low_level_api/ck_low_level_mem_gen.cpp \
	catcake/source/low_level_api/ck_low_level_util_freetype.cpp \
	catcake/source/low_level_api/ck_low_level_util_libpng.cpp \
	catcake/source/low_level_api/iphone/ck_low_level_snd_iphone.cpp \
	catcake/source/low_level_api/iphone/ck_low_level_sys_iphone.cpp \
	catcake/source/low_level_api/linux/ck_low_level_snd_linux.cpp \
	catcake/source/low_level_api/linux/ck_low_level_sys_linux.cpp \
	catcake/source/low_level_api/macosx/ck_low_level_snd_macosx.cpp \
	catcake/source/low_level_api/macosx/ck_low_level_sys_macosx.cpp \
	catcake/source/low_level_api/win32/ck_low_level_snd_win32.cpp \
	catcake/source/low_level_api/win32/ck_low_level_sys_win32.cpp \
	catcake/source/math/ck_fix.cpp \
	catcake/source/math/ck_mat.cpp \
	catcake/source/math/ck_math.cpp \
	catcake/source/math/ck_quat.cpp \
	catcake/source/math/ck_vec.cpp \
	catcake/source/mem/ck_mem_mgr.cpp \
	catcake/source/mem/ck_mem_util.cpp \
	catcake/source/res/ck_res.cpp \
	catcake/source/res/ck_res_mgr.cpp \
	catcake/source/snd/ck_snd_mgr.cpp \
	catcake/source/snd/ck_snd_mgr_mix.cpp \
	catcake/source/sys/ck_sys_mgr.cpp \
	catcake/source/sys/ck_sys_util.cpp \
	catcake/source/task/ck_task.cpp \
	catcake/source/task/ck_task_mgr.cpp \
	catcake/source/util/ck_util.cpp \
	catcake/source/util/ck_util_import_3ds.cpp \
	catcake/source/util/ck_util_load_pixel_art_model.cpp

LOCAL_CXXFLAGS := -Icatcake/include

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_STATIC_LIBRARY)
