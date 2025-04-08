import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'

function App() {
  const [count, setCount] = useState(0)

  return (
    <div className='container'>
      <a  href='https://github.com/AKASH-KUMAR-R/lab-usage/archive/refs/heads/main.zip'>
        <div className='link'>
          Click herre
        </div>
      </a>
    </div>
  )
}

export default App
