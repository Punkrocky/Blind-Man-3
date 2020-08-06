/*******************************************************************************************************************//**
* \file Serializer.h
* \author Matthew LaDouceur
* \date 7-28-2020
* \brief Header for Serializer
 **********************************************************************************************************************/
#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <vector>

namespace pt = boost::property_tree;

class Serializer
{
public:
  enum class Mode
  {
    Read,
    Write
  };

  Serializer(std::string fileName, Mode mode)
  {
    FileName = fileName;

    if (mode == Mode::Read)
    {
      pt::read_json(FileName, root);

      for (const pt::ptree::value_type& key : root)
      {
        // Count the number of entries in the file
        ++ItemCount;
      }
    }
  }

  ~Serializer() {}

  void Useless()
  {
    // Serialize several individual values
    pt::ptree under;
    {
      under.put("Item.Value.Height", 10);
      under.put("Item2.Value2.Height2", 120);
      under.put("Item2.Value2.Height3", 120);
    }
    root.add_child("0", under);

    // Serialize an array of values
    pt::ptree Values_tree;
    {
      int Values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      pt::ptree Num_tree;
      for (const auto& num : Values)
      {
        pt::ptree Num_Node;

        Num_Node.put("", num);

        Num_tree.push_back(std::make_pair("", Num_Node));
      }
      Values_tree.add_child("Array", Num_tree);
    }
    root.add_child("1", Values_tree);

    // Serialize a matrix, an array of arrays
    pt::ptree Indent_tree;
    {
      int Matrix[][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
      pt::ptree Matrix_tree;
      for (const auto& col : Matrix)
      {
        pt::ptree Col_Node;
        for (const auto& row : col)
        {
          pt::ptree Row_Node;
          Row_Node.put("", row);

          Col_Node.push_back(std::make_pair("", Row_Node));
        }
        Matrix_tree.push_back(std::make_pair("", Col_Node));
      }
      Indent_tree.add_child("Matrix", Matrix_tree);
    }
    root.add_child("2", Indent_tree);

    root.put("0.Size", 3.14f);

    boost::property_tree::write_json("TempJSON.json", root);
  }


  template <typename T> void WriteJSON(T obj);
  template <typename T> bool ReadJSON(T& obj);

private:
  pt::ptree root;
  std::string FileName;
  int ItemCount = 0;
  int ItemIndex = 0;
};


template <typename T> void Serializer::WriteJSON(T obj)
{
  // Add an entry to serialize. ItemCount will hold the entry number.
  root.add_child(std::to_string(ItemCount++), obj.Save());
}


template <typename T> bool Serializer::ReadJSON(T& obj)
{
  // Keep track of how many items we've pulled out of JSON.
  if (ItemIndex == ItemCount)
  {
    return false;
  }
  else
  {
    obj.Load(root.get_child(std::to_string(ItemIndex++)));
    return true;
  }
}
