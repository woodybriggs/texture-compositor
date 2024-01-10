#version 330;

in mediump vec2 vert_uv;

uniform sampler2D idmap_texture;

uniform sampler2D texid_albedo[3];
uniform sampler2D texid_metallic[3];
uniform sampler2D texid_roughness[3];

out mediump vec4 FragColor;

mediump vec4 texid_texture(sampler2D id, mediump vec2 uv, sampler2D r, sampler2D g, sampler2D b) {
  mediump vec4 col_id = texture(id, uv);
  mediump vec4 r_sample = texture(r, uv);
  mediump vec4 g_sample = texture(g, uv);
  mediump vec4 b_sample = texture(b, uv);

  mediump vec3 r_contrib = mix(col_id.rrr, r_sample.rgb, col_id.r);
  mediump vec3 g_contrib = mix(col_id.ggg, g_sample.rgb, col_id.g);
  mediump vec3 b_contrib = mix(col_id.bbb, b_sample.rgb, col_id.b);

  return vec4(r_contrib + g_contrib + b_contrib, 1.0);
}

void main() {

  mediump vec4 id_map = texture(idmap_texture, vert_uv);
  mediump vec4 albedo = texid_texture(idmap_texture, vert_uv, texid_albedo[0], texid_albedo[1], texid_albedo[2]);
  mediump vec4 metallic = texid_texture(idmap_texture, vert_uv, texid_metallic[0], texid_metallic[1], texid_metallic[2]);
  mediump vec4 roughness = texid_texture(idmap_texture, vert_uv, texid_roughness[0], texid_roughness[1], texid_roughness[2]);

  FragColor = albedo;
}