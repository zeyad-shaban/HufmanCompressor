import { Container, Row, Col, Card, Button, Form } from 'react-bootstrap';
import React, { useState } from 'react';

function BasicExample({ compressed_file, decoder_map ,frequency_table}) {
    

    // Function to download text as a file
    const downloadText = (text, filename) => {
        const element = document.createElement('a');
        const file = new Blob([text], { type: 'text/plain' });
        element.href = URL.createObjectURL(file);
        element.download = filename;
        document.body.appendChild(element); // Required for Firefox
        element.click();
        document.body.removeChild(element); // Clean up
    };

    return (
        <Container>
            <Row style={{ marginTop: "30px" }}>
                <Col xs={12} md={6} lg={3}>
                    <Card style={{ width: '18rem' }}>
                        <Card.Body>
                            <Card.Title>Frequency Table</Card.Title>
                            <Form.Control as="textarea" rows={3} value={frequency_table}readOnly />
                            <Card.Text>
                                Some quick example text to build on the card title and make up the
                                bulk of the card's content.
                            </Card.Text>
                            <Button variant="success" onClick={() => downloadText(frequency_table, 'frequency_table.txt')}>Download</Button>
                        </Card.Body>
                    </Card>
                </Col>
                <Col xs={12} md={6} lg={3}>
                    <Card style={{ width: '18rem' }}>
                        <Card.Body>
                            <Card.Title>Compressed Txt</Card.Title>
                            <Form.Control as="textarea" rows={3} value={compressed_file} readOnly />
                            <Card.Text>
                                Some quick example text to build on the card title and make up the
                                bulk of the card's content.
                            </Card.Text>
                            <Button variant="success" onClick={() => downloadText(compressed_file, 'compressed_txt.com')}>Download</Button>
                        </Card.Body>
                    </Card>
                </Col>
                <Col xs={12} md={6} lg={3}>
                    <Card style={{ width: '18rem' }}>
                        <Card.Body>
                            <Card.Title>Decoded Map</Card.Title>
                            <Form.Control as="textarea" rows={3} value={decoder_map} readOnly />
                            <Card.Text>
                                Some quick example text to build on the card title and make up the
                                bulk of the card's content.
                            </Card.Text>
                            <Button variant="success" onClick={() => downloadText(decoder_map, 'decoded_map.json')}>Download</Button>
                        </Card.Body>
                    </Card>
                </Col>
            </Row>
        </Container>
    );
}

export default BasicExample;
