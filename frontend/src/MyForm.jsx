import React, { useState } from 'react';
import { Form, Button, FormControl } from 'react-bootstrap';

const MyForm = ({ onDataUpdate }) => {
 const handleSubmit = (event) => {
    event.preventDefault();
    const target = event.target;
    const buttonClicked = target.querySelector('button[type="submit"]:focus');

    if (buttonClicked.name === 'textSubmit') {
      // Handle text submit
      const textValue = target.querySelector('#formHorizontalText').value;
      console.log('Text Submit:', textValue);
      // Send the text value to the server
      sendDataToServer({ message: textValue });
    } else if (buttonClicked.name === 'fileSubmit') {
      // Handle file submit
      const fileInput = target.querySelector('#formFile');
      if (fileInput.files.length > 0) {
        const file = fileInput.files[0];
        console.log('File Submit:', file);
        // Read the file as text
        const reader = new FileReader();
        reader.onload = (e) => {
          // The file's text will be printed here
          console.log(e.target.result);
          // Send the file's text to the server
          sendDataToServer({ message: e.target.result });
        };
        reader.readAsText(file);
      }
    }
 };

 const sendDataToServer = (data) => {
    const url = 'http://localhost:8080/compress';
    fetch(url, {
      method: 'POST', // or 'PUT'
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(data),
    })
    .then(response => response.json())
    .then(data => {
      console.log('Success:', data);
      onDataUpdate(data);
    })
    .catch((error) => {
      console.error('Error:', error);
    });
 };

 return (
    <div style={{ display: "flex", justifyContent: "center", backgroundColor: "#135D66" }}>
      <Form className="text-white" onSubmit={handleSubmit} style={{ display: 'flex', flexDirection: 'row', alignItems: 'center', justifyContent: 'flex-start' }}>
        <Form.Group controlId="formHorizontalText" style={{ marginRight: '10px', marginLeft: "10px" }}>
          <div style={{ display: "flex", justifyContent: "space-between" }}>
            <Form.Label style={{ fontSize: "14px" }}>Input Text:</Form.Label>
            <Button variant="outline-light" type="submit" name="textSubmit" size="sm" style={{ marginBottom: "7px", fontSize: "14px" }}>Submit Text</Button>
          </div>
          <Form.Control type="text" placeholder="Enter text" style={{ fontSize: "14px" }} />
        </Form.Group>

        <Form.Group controlId="formFile" className="mb-3" style={{ marginRight: '10px', marginLeft: "10px" }}>
          <div style={{ display: "flex", justifyContent: "space-between", marginTop: '14px' }}>
            <Form.Label style={{ fontSize: "14px" }}>Upload File:</Form.Label>
            <Button variant="outline-light" type="submit" name="fileSubmit" size="sm" style={{ marginBottom: "7px", fontSize: "14px" }}>Submit File</Button>
          </div>
          <Form.Control type="file" style={{ fontSize: "14px" }} />
        </Form.Group>
      </Form>
    </div>
 );
};

export default MyForm;