import React from 'react';
import { Navbar, Container } from 'react-bootstrap';
import './assets/header.css'
const Header = () => {
 return (
    <Navbar  expand="lg" className="custom-dark-blue">
      <Container>
        <Navbar.Brand href="#home" className="text-white">Huffman Txt File Compresser & DeCompresser</Navbar.Brand>
      </Container>
    </Navbar>
 );
};

export default Header;
