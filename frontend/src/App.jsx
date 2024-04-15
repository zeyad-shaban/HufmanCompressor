import React, { useState } from 'react';
import Header from './Header';
import MyForm from './MyForm';
import MainContent from './MainContent';

const App = () => {
 const [data, setData] = useState(null);

 const handleDataUpdate = (newData) => {
    setData(newData);
 };

 return (
    <>
      <Header />
      <MyForm onDataUpdate={handleDataUpdate} />
      <MainContent data={data} />
    </>
 );
};

export default App;
