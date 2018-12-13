vector3d.o: src/vector3d.c src/vector3d.h
view.o: src/view.h src/vector3d.h
vector4d.o: src/vector4d.h
transform3d.o: src/transform3d.h src/vector3d.h src/vector4d.h
vector2d.o: src/vector2d.h
image2d.o: src/image2d.h src/transform2d.h src/vector2d.h src/vector4d.h \
 src/view.h src/vector3d.h
transform2d.o: src/transform2d.h src/vector2d.h src/vector4d.h
vector3d.o: src/vector3d.h
