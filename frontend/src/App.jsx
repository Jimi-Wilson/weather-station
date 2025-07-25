import './App.css'
import WeatherBlocks from "./components/WeatherBlocks.jsx";
import WeatherDisplay from "./components/WeatherDisplay.jsx";

function App() {

  return (
      <div className="flex flex-col justify-center items-center">
          <h1 className="">Matlock Weather Station Dashboard</h1>
          <WeatherDisplay />
      </div>
  )
}


export default App
