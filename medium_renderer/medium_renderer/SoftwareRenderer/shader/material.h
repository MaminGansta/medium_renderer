#pragma once

#include "image/image.h"

struct Material
{
	float ambient;
	float shanest;
	float specular;

	union
	{
		gui::Image* diffusemap;
		gui::Color diffuse;
	};

	gui::fImage* specularmap;
	gui::fImage* normalmap;
	gui::fImage* heightmap;

	bool diffuse_flag = false;
	bool specular_flag = false;
	bool normal_flag = false;
	bool height_flag = false;


	inline Material()
	{
		// flush unions
		memset(this, 0, sizeof(Material));
		ambient = 0.1f;
		specular = 0.5f;
		shanest = 32.0f;
		diffuse = gui::Color(255);
	}

	inline void set_diffusemap(gui::Image* diffmap)
	{
		diffusemap = diffmap;
		diffuse_flag = diffmap->data;
	}

	inline void set_specularmap(gui::fImage* specmap)
	{
		specularmap = specmap;
		specular_flag = specmap ? specmap->data : false;
	}

	inline void set_normalmap(gui::fImage* normmap)
	{
		normalmap = normmap;
		normal_flag = normalmap ? normmap->data : false;
	}

	inline void set_heightmap(gui::fImage* hmap)
	{
		heightmap = hmap;
		height_flag = hmap ? hmap->data : false;
	}



	inline void set_ambient(float ambint_streangth)
	{
		ambient = ambint_streangth;
	}

	inline void set_diffuse(const gui::Color& diffuse_color)
	{
		diffuse = diffuse_color;
		diffuse_flag = false;
	}

	inline void set_specular(float specular_strenght)
	{
		specular = specular_strenght;
		specular_flag = false;
	}

};