import React from 'react';
import Tree from 'react-d3-tree';
import './assets/huffman.css'

const HuffmanTree = ({huffmanTree}) => {
 const transformHuffmanTree = (node, parentId = null) => {
  if (!node) return null;
 
  const id = parentId ? `${parentId}-${node.letters}` : node.letters;
 
  return {
     id,
     name: `${node.letters} (${node.freq})`,
     children: [
       transformHuffmanTree(node.left, id),
       transformHuffmanTree(node.right, id),
     ].filter(Boolean),
  };
 };
 

 const treeData = transformHuffmanTree(huffmanTree);

 return (
    <div className="App">

    <div className="tree-container">
      <Tree
        data={treeData}
        translate={{ x: 600, y: 50 }} // Adjust the x and y values as needed
        orientation="vertical"
        scaleExtent={{ min: 0.1, max: 1 }}
        />
    </div>
    </div>
);
};

export default HuffmanTree;
