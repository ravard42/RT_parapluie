/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_inter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: owinckle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 11:54:41 by owinckle          #+#    #+#             */
/*   Updated: 2017/05/23 02:11:16 by dbirtel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_val	get_val(t_ray r, float rad)
{
	t_val	v;
	float	a;

	a = tan(rad);
	v.param[0] = pow(r.dir.x, 2.0) + pow(r.dir.z, 2.0) - pow(r.dir.y, 2.0) * a;
	v.param[1] = 2.0 * r.dir.x * r.ori.x + 2.0 * r.dir.z *
		r.ori.z - 2.0 * r.dir.y * r.ori.y * a;
	v.param[2] = pow(r.ori.x, 2.0) + pow(r.ori.z, 2.0) - pow(r.ori.y, 2.0) * a;
	v.det = v.param[1] * v.param[1] - 4.0 * v.param[0] * v.param[2];
	if (v.det > 0)
	{
		v.r[0] = (-v.param[1] + sqrt(v.det)) / (2 * v.param[0]);
		v.r[1] = (-v.param[1] - sqrt(v.det)) / (2 * v.param[0]);
		if (v.r[1] < v.r[0])
		{
			v.r[2] = v.r[0];
			v.r[0] = v.r[1];
			v.r[1] = v.r[2];
		}
	}
	return (v);
}

int				void_cone_inter(t_vect hit, t_obj *o)
{
	t_ray	r;
	t_val	v;
	t_vect	bam;
	float	a;

	r.ori = hit;
	r.dir = vec_norm(sub_coord(hit, o->ori));
	r = move_ray(r, o);
	bam = (t_vect){0, 0, 0};
	bam.y = r.ori.y;
	r.dir = vec_norm(sub_coord(r.ori, bam));
	a = tan(o->rad);
	v.param[0] = pow(r.dir.x, 2.0) + pow(r.dir.z, 2.0) - pow(r.dir.y, 2.0) * a;
	v.param[1] = 2.0 * r.dir.x * r.ori.x + 2.0 *
		r.dir.z * r.ori.z - 2.0 * r.dir.y * r.ori.y * a;
	v.param[2] = pow(r.ori.x, 2.0) + pow(r.ori.z, 2.0) - pow(r.ori.y, 2.0) * a;
	v.det = v.param[1] * v.param[1] - 4.0 * v.param[0] * v.param[2];
	if (v.det < 0)
		return (0);
	v.r[0] = (-v.param[1] + sqrt(v.det)) / (2 * v.param[0]);
	v.r[1] = (-v.param[1] - sqrt(v.det)) / (2 * v.param[0]);
	if ((v.r[0] > 0 && v.r[1] < 0) || (v.r[0] < 0 && v.r[1] > 0))
		if (in_lim(r.ori, o))
			return (1);
	return (0);
}

void			cone_inter(t_ray *t, t_obj *o)
{
	t_val	v;
	t_ray	r;

	r = move_ray(*t, o);
	v = get_val(r, o->rad);
	if (v.det < 0)
		return ;
	r.p[0] = translate(r.ori, r.dir, v.r[0]);
	r.p[1] = translate(r.ori, r.dir, v.r[1]);
	if (v.r[0] > 0 && v.r[0] < t->inter && in_lim(r.p[0], o))
	{
		if (rlim(t->ori, t->dir, v.r[0], o->l) && !hit_void(r.p[0], o))
			load_ray_plus(t, v.r[0], r.p[0], o);
		else
		{
			if (v.r[1] > 0 && v.r[1] < t->inter && in_lim(r.p[1], o))
				if (rlim(t->ori, t->dir, v.r[1], o->l) && !hit_void(r.p[1], o))
					load_ray(t, v.r[1], r.p[1], o);
		}
	}
	else if (v.r[1] > 0 && v.r[1] < t->inter && in_lim(r.p[1], o))
	{
		if (rlim(t->ori, t->dir, v.r[1], o->l) && !hit_void(r.p[1], o))
			load_ray(t, v.r[1], r.p[1], o);
	}
}
