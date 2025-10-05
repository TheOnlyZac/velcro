
# Accept a version argument (default: 1.0.0)
param(
	[string]$Version = '1.0.0'
)

# Normalize version string by removing any leading 'v'
if ($Version.StartsWith('v')) {
	$Version = $Version.Substring(1)
}

# Compose package name and paths using version
$packageName = "velcro-$Version-win-x64"

# Change to the directory above the script
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location (Join-Path $scriptDir '..')

# Make dist directory if it doesn't exist
if (-not (Test-Path 'dist')) {
    New-Item -ItemType Directory -Path 'dist' | Out-Null
}

# Delete the build directory if it exists
if (Test-Path 'build') {
	Remove-Item 'build' -Recurse -Force
}

# Create the build directory
New-Item -ItemType Directory -Path 'build' | Out-Null

# Run build commands
Set-Location 'build'
cmake ..
cmake --build . --config Release
Set-Location '..'

# Rename bin\Release to bin\bin (preserve existing behavior)
$releasePath = 'build/bin/Release'
$binPath = 'build/bin/bin'
if (Test-Path $releasePath) {
	if (Test-Path $binPath) { Remove-Item $binPath -Recurse -Force }
	Move-Item $releasePath $binPath
}

# Copy docs to bin\docs
$docsSrc = 'docs'
$docsDst = 'build/bin/docs'
if (Test-Path $docsDst) { Remove-Item $docsDst -Recurse -Force }
Copy-Item $docsSrc $docsDst -Recurse

# Copy samples to bin\samples
$samplesSrc = 'samples'
$samplesDst = 'build/bin/samples'
if (Test-Path $samplesDst) { Remove-Item $samplesDst -Recurse -Force }
Copy-Item $samplesSrc $samplesDst -Recurse

# Concatenate all packages/*/README.md into bin/README.md
$readmeFiles = Get-ChildItem -Path 'packages/*/README.md' -File
$outReadme = 'build/bin/README.md'
if (Test-Path $outReadme) { Remove-Item $outReadme }
foreach ($file in $readmeFiles) {
	Get-Content $file | Add-Content $outReadme
	Add-Content $outReadme "`n`n" # Separate each README
}

# Add top-level heading "Velcro v<version>" and increase all other headings by one level
$readmeContent = Get-Content $outReadme
$updatedContent = @()
$updatedContent += "# Velcro v$Version`n"
foreach ($line in $readmeContent) {
    if ($line -match '^(#+)(.*)') {
        $hashes = $matches[1]
        $text = $matches[2]
        $newHashes = $hashes + '#'
        $updatedContent += "$newHashes$text"
    } else {
        $updatedContent += $line
    }
}
Set-Content -Path $outReadme -Value $updatedContent

# Rename build/bin to build/<packageName>
$finalDir = Join-Path 'build' $packageName
if (Test-Path $finalDir) { Remove-Item $finalDir -Recurse -Force }
Move-Item 'build/bin' $finalDir

# Create a zip file of the directory
$zipPath = Join-Path 'build' ("$packageName.zip")
if (Test-Path $zipPath) { Remove-Item $zipPath }
Compress-Archive -Path $finalDir -DestinationPath $zipPath

# Move the zip file to dist/
$distPath = Join-Path 'dist' ("$packageName.zip")
if (Test-Path $distPath) { Remove-Item $distPath }
Move-Item $zipPath $distPath
