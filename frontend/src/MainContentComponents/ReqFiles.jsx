import { Container, Row, Col, Card,Button,Form } from 'react-bootstrap';
import React, { useState } from 'react';

function BasicExample({compressed_file,decoder_map}) {
    

   const tringifiedArray = decoder_map.map(obj => `${obj.value}:${obj.key}`).join(',\n');


    return (
        <Container>
            <Row  style={{marginTop:"30px"}}>
                <Col xs={12} md={6} lg={3}>
                    <Card style={{ width: '18rem' }}>
                        <Card.Body>
                            <Card.Title>Frequency Table</Card.Title>
                    <Form.Control as="textarea" rows={3} readOnly/>
                            <Card.Text>
                                Some quick example text to build on the card title and make up the
                                bulk of the card's content.
                            </Card.Text>
                            <Button variant="success">Download</Button>
                        </Card.Body>
                    </Card>
                    
                </Col>
                <Col xs={12} md={6} lg={3}>
                <Card style={{ width: '18rem' }} >
                        <Card.Body>
                            <Card.Title>Compressed Txt</Card.Title>
                    <Form.Control as="textarea" rows={3} value={compressed_file} readOnly/>
                            <Card.Text>
                                Some quick example text to build on the card title and make up the
                                bulk of the card's content.
                            </Card.Text>
                            <Button variant="success">Download</Button>
                        </Card.Body>
                    </Card>
                </Col>
                <Col xs={12} md={6} lg={3}>
                <Card style={{ width: '18rem' }} >
                        <Card.Body>
                            <Card.Title>Decoded Map</Card.Title>
                    <Form.Control as="textarea" rows={3} value={tringifiedArray}/>
                            <Card.Text>
                                Some quick example text to build on the card title and make up the
                                bulk of the card's content.
                            </Card.Text>
                            <Button variant="success">Download</Button>
                        </Card.Body>
                    </Card>
                </Col>

            </Row>
            
        </Container>
    );
}

export default BasicExample;