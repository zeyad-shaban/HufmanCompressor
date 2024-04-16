import React, { useEffect } from 'react';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import Tab from 'react-bootstrap/Tab';
import Tabs from 'react-bootstrap/Tabs';

import ReqFiles from './MainContentComponents/ReqFiles.jsx'
import HuffmanTree from './HuffmanTree'
import MyForm2 from './MainContentComponents/MyForm2.jsx';
import Compress from './MainContentComponents/Compress.jsx';


const MainContent = ({ data }) => {


    return (
        <Tabs
            defaultActiveKey="profile"
            id="fill-tab-example"
            className="mb-3"
            fill
        >
            <Tab eventKey="files" title="Requested Files">
                {data ? (

                    <ReqFiles compressed_file={data.compressed_file} decoder_map={JSON.stringify(data.decoder_map)} frequency_table = {data.frequency_table}/>
                ) : (
                    <p>no Data received yet</p>
                )}
            </Tab>
            <Tab eventKey="profile" title="Huffman Tree Visualization">
                {data ? (

                    <HuffmanTree huffmanTree={data.huffman_tree}/>

                ) : (
                    <p>no Data received yet</p>
                )}
            </Tab>
            <Tab eventKey="longer-tab" title="DeCompress">
                <Compress/>
            </Tab>
        </Tabs>
    );
}

export default MainContent