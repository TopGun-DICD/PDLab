#pragma once

#include <string>
#include <vector>

struct Units {
  double  user,
          physical;
};

struct Coord {
  __int32 x,
          y;
};

struct Property {
  __int16     index;
  std::string value;
};

enum class ItemType {
  undefined = 0,
  boundary,
  path,
  text,
  box,
  structRef,
};

enum class DataType {
  noData = 0,
  bitArray,
  WORD,
  DWORD,
  REAL,
  DOUBLE,
  ASCIISTRING,
};

enum class FileFormat {
  undefined = 0,
  GDSII_bin,
  GDSII_ASCII,
  MSK,
};

struct GeometryItem {
  ItemType                    type;
  __int16                     layer;
  std::vector<Property>       properties;
  std::vector<Coord>          coords;
};

struct GeometryItem_Boundary : public GeometryItem {
  __int16                     dataType;
};

struct GeometryItem_Path : public GeometryItem {
  __int16                     dataType,
                              pathType;
  __int32                     width;
};

struct GeometryItem_Text : public GeometryItem {
  __int16                     textType,
                              flagsPresentation,
                              pathType,
                              flagsTransformation;
  double                      magnification;
  __int32                     width;
  std::string                 stringValue;
};

struct GeometryItem_Box : public GeometryItem {
  __int16                     boxType;
};

struct Element;

struct GeometryItem_StructureRef : public GeometryItem {
  std::string                 name;
  Element                    *pReference;
  __int16                     transformationFlags;
  double                      magnification;
};

struct Element {
  std::string                 name;
  std::vector<GeometryItem *> items;
};

struct Layer {
  __int16                     layer;
  std::string                 name;
  std::vector<GeometryItem *> items;
};

struct Library {
  std::string                 name;
  Units                       units;
  std::vector<Element *>      elements;
  std::vector<Layer>          layers;
};

struct Layout {
  std::wstring            fileName;
  FileFormat              fileFormat;
  std::vector<Library *>  libraries;
};

typedef Layout *(*Func_CreateLayout)  (std::wstring);
typedef void    (*Func_ReadLayout)    (std::wstring, Layout *);
typedef void    (*Func_ClearLayout)   (Layout *);
typedef void    (*Func_CopyLayout)    (Layout *, Layout *);
typedef void    (*Func_FreeLayout)    (Layout **);

__declspec(dllexport) Layout *CreateLayoutFromFile(std::wstring fileName);
__declspec(dllexport) void ReadLayoutFromFile(std::wstring fileName, Layout *layout);
__declspec(dllexport) void ClearLayout(Layout *layout);
__declspec(dllexport) void CopyLayout(Layout *src, Layout *dst);
__declspec(dllexport) void FreeLayout(Layout **layout);
