#
#
#


LOCAL_PATH := $(call my-dir)

BASE_DIR = $(LOCAL_PATH)/../../..

include $(CLEAR_VARS)

LOCAL_MODULE := $(BASE_DIR)

LOCAL_SRC_FILES := \
	$(BASE_DIR)/source/cdt/ck_cdt_aabb.cpp \
	$(BASE_DIR)/source/cdt/ck_cdt_box.cpp \
	$(BASE_DIR)/source/cdt/ck_cdt_ray.cpp \
	$(BASE_DIR)/source/cdt/ck_cdt_sph.cpp \
	$(BASE_DIR)/source/cdt/ck_cdt_tri.cpp \
	$(BASE_DIR)/source/conf/ck_conf.cpp \
	$(BASE_DIR)/source/conf/ck_conf_mgr.cpp \
	$(BASE_DIR)/source/conf/ck_conf_parse.cpp \
	$(BASE_DIR)/source/conf/ck_ent.cpp \
	$(BASE_DIR)/source/dbg/ck_dbg_mgr.cpp \
	$(BASE_DIR)/source/dbg/ck_dbg_mgr_draw.cpp \
	$(BASE_DIR)/source/dbg/ck_dbg_mgr_dump.cpp \
	$(BASE_DIR)/source/dbg/ck_dbg_mgr_font.cpp \
	$(BASE_DIR)/source/draw/ck_col.cpp \
	$(BASE_DIR)/source/draw/ck_draw.cpp \
	$(BASE_DIR)/source/draw/ck_draw_mgr.cpp \
	$(BASE_DIR)/source/draw/ck_font.cpp \
	$(BASE_DIR)/source/draw/ck_lit.cpp \
	$(BASE_DIR)/source/draw/ck_lts.cpp \
	$(BASE_DIR)/source/draw/ck_mdl.cpp \
	$(BASE_DIR)/source/draw/ck_mdl_data.cpp \
	$(BASE_DIR)/source/draw/ck_mot.cpp \
	$(BASE_DIR)/source/draw/ck_mot_data.cpp \
	$(BASE_DIR)/source/draw/ck_node.cpp \
	$(BASE_DIR)/source/draw/ck_prim.cpp \
	$(BASE_DIR)/source/draw/ck_rend.cpp \
	$(BASE_DIR)/source/draw/ck_scr.cpp \
	$(BASE_DIR)/source/draw/ck_shd.cpp \
	$(BASE_DIR)/source/draw/ck_sprt.cpp \
	$(BASE_DIR)/source/draw/ck_tex.cpp \
	$(BASE_DIR)/source/draw/rend/ck_rend_light.cpp \
	$(BASE_DIR)/source/draw/rend/ck_rend_shader.cpp \
	$(BASE_DIR)/source/gen/ck_id.cpp \
	$(BASE_DIR)/source/key/ck_key_mgr.cpp \
	$(BASE_DIR)/source/low_level_api/android/ck_low_level_sys_android.cpp \
	$(BASE_DIR)/source/low_level_api/ck_low_level_draw_opengl.cpp \
	$(BASE_DIR)/source/low_level_api/ck_low_level_io_gen.cpp \
	$(BASE_DIR)/source/low_level_api/ck_low_level_math_gen.cpp \
	$(BASE_DIR)/source/low_level_api/ck_low_level_mem_gen.cpp \
	$(BASE_DIR)/source/low_level_api/ck_low_level_util_freetype.cpp \
	$(BASE_DIR)/source/low_level_api/ck_low_level_util_libpng.cpp \
	$(BASE_DIR)/source/low_level_api/iphone/ck_low_level_snd_iphone.cpp \
	$(BASE_DIR)/source/low_level_api/iphone/ck_low_level_sys_iphone.cpp \
	$(BASE_DIR)/source/low_level_api/linux/ck_low_level_snd_linux.cpp \
	$(BASE_DIR)/source/low_level_api/linux/ck_low_level_sys_linux.cpp \
	$(BASE_DIR)/source/low_level_api/macosx/ck_low_level_snd_macosx.cpp \
	$(BASE_DIR)/source/low_level_api/macosx/ck_low_level_sys_macosx.cpp \
	$(BASE_DIR)/source/low_level_api/win32/ck_low_level_snd_win32.cpp \
	$(BASE_DIR)/source/low_level_api/win32/ck_low_level_sys_win32.cpp \
	$(BASE_DIR)/source/math/ck_fix.cpp \
	$(BASE_DIR)/source/math/ck_mat.cpp \
	$(BASE_DIR)/source/math/ck_math.cpp \
	$(BASE_DIR)/source/math/ck_quat.cpp \
	$(BASE_DIR)/source/math/ck_vec.cpp \
	$(BASE_DIR)/source/mem/ck_mem_mgr.cpp \
	$(BASE_DIR)/source/mem/ck_mem_util.cpp \
	$(BASE_DIR)/source/res/ck_res.cpp \
	$(BASE_DIR)/source/res/ck_res_mgr.cpp \
	$(BASE_DIR)/source/snd/ck_snd_mgr.cpp \
	$(BASE_DIR)/source/snd/ck_snd_mgr_mix.cpp \
	$(BASE_DIR)/source/sys/ck_sys_mgr.cpp \
	$(BASE_DIR)/source/sys/ck_sys_util.cpp \
	$(BASE_DIR)/source/task/ck_task.cpp \
	$(BASE_DIR)/source/task/ck_task_mgr.cpp \
	$(BASE_DIR)/source/util/ck_util.cpp \
	$(BASE_DIR)/source/util/ck_util_import_3ds.cpp \
	$(BASE_DIR)/source/util/ck_util_load_pixel_art_model.cpp

LOCAL_CXXFLAGS := \
	-DNDEBUG -DCK_LINUX \
	-DCK_NO_THROW_EXCEPTION \
	-I$(BASE_PATH)/include \
	-I$(BASE_PATH)/source
	-I$(BASE_DIR)/project/include/zlib \
	-I$(BASE_DIR)/project/include/libpng \
	-I$(BASE_DIR)/project/include/freetype

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_STATIC_LIBRARY)
